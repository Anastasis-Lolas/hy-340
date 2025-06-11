#include "../../Symtable/symtable.h"
#include "../avm_execute.h"
#include "../library_functions.h"

#define DEBUG_check(msg) std::cout << "[DEBUG]: " << msg << std::endl;
// #define DEBUG_check(msg)

extern avm_memcell stack[AVM_STACKSIZE];
SymTable_T libFuncs;

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    switch (func->type) {
        case userfunc_m: {
            avm_callsaveenvironment();  // Maybe outside switch?
            pc = func->funcVal;
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            break;
        }
        case string_m:
            avm_callibfunc(func->strVal);
            break;
        case libfunc_m:
            DEBUG_check("Calling library function: " + func->libfuncVal);
            avm_callibfunc(func->libfuncVal);
            break;
        case table_m:
            avm_call_functor(func->tableVal);
            break;

        default:
            std::string err_msg =
                "Call: cannot bind " + avm_toString(func) + " to function!";
            avm_error(err_msg);
            executionFinished = 1;
    }
}
void avm_call_functor(avm_table* table) {  // flag edw
    cx.type = string_m;
    cx.strVal = "()";

    avm_memcell* f = avm_tablegetelem(table, &cx);
    if (!f)
        avm_error("in calling table: no '()' element found!");
    else if (f->type == table_m) {
        avm_call_functor(f->tableVal);
    } else if (f->type == userfunc_m) {
        avm_push_table_arg(table);
        avm_callsaveenvironment();
        pc = f->funcVal;
        assert(pc < AVM_ENDING_PC && code[pc].opcode == funcenter_v);
    } else {
        avm_error("in calling table: illegal '()' element value!");
    }
}

void avm_callibfunc(std::string funcName) {
    library_func_t f = avm_getlibraryfunc(funcName);
    if (!f) {
        avm_error("unsupported library function '" + funcName + "'called!");
        executionFinished = 1;
    } else {
        avm_callsaveenvironment();
        topsp = top;
        totalActuals = 0;
        (*f)();
        if (!executionFinished) {
            execute_funcexit((instruction*)0);
        }
    }
}

void execute_pusharg(instruction* instr) {
    avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
    assert(arg);

    avm_assign(&stack[top], arg);
    ++totalActuals;
    avm_dec_top();
}

void execute_funcenter(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    assert(pc == func->funcVal); /* Func address should match PC. */

    totalActuals = 0;
    userfunc* f = avm_getfuncinfo(func->funcVal);
    topsp = top;
    top = top - f->localSize;
}
void execute_funcexit(instruction*) {
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    while (++oldTop < top) { /* Intentionally ignoring first. */
        avm_memcellclear(&stack[oldTop]);
    }
}

unsigned avm_get_envvalue(unsigned i) {
    assert(stack[i].type == number_m);
    unsigned val = (unsigned)stack[i].numVal;
    assert(stack[i].numVal == ((double)val));
    return val;
}

userfunc* avm_getfuncinfo(unsigned address) {
    for (size_t i = 0; i < userfuncs.size(); ++i) {
        if (userfuncs[i].address == address) {
            return &userfuncs[i];
        }
    }
    avm_error("Function with address " + std::to_string(address) +
              " not found!");
    return nullptr;
}

unsigned avm_totalactuals(void) {
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i) {
    assert(i < avm_totalactuals());
    return &stack[top + AVM_STACKENV_SIZE + 1 + i];
}

void avm_registerlibfunc(std::string id, library_func_t addr) {
    if (SymTable_get(libFuncs, id)) {
        avm_error("Library function '" + id + "' already registered!");
    } else {
        SymTable_put(libFuncs, id, (void*)addr);
    }
}

void libfunc_typeof() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("typeof expects exactly one argument, but got " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }
    avm_memcellclear(&retval);
    retval.type = string_m;
    retval.strVal = typeStrings[avm_getactual(0)->type];
}
void libfunc_totalarguments() {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (!p_topsp) {
        avm_error("totalarguments called outside of a function!");
        retval.type = nil_m;
    } else {
        retval.type = number_m;
        retval.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

library_func_t avm_getlibraryfunc(std::string id) {
    return (library_func_t)SymTable_get(libFuncs, id);
}

// checkpoint for alex

void libfunc_print() {
    unsigned n = avm_totalactuals();
    for (unsigned i = 0; i < n; ++i) {
        avm_memcell* m = avm_getactual(i);
        if (m->type == userfunc_m) {
            userfunc* f = avm_getfuncinfo(m->funcVal);
            if (f) {
                std::cout << "User Function: " << f->id << " at address "
                          << f->address << "\n";
            } else {
                std::cout << "User Function: Unknown at address " << m->funcVal
                          << "\n";
            }
        } else if (m->type == libfunc_m) {
            std::cout << "Library Function: " << m->libfuncVal << "\n";
        } else if (m->type == table_m) {
            // Assuming avm_table has a toString method or similar flag gia evi
            assert(0);
        } else {
            std::cout << avm_toString(m) << "\n";
        }
    }
}
void libfunc_input() {
    unsigned n = avm_totalactuals();
    if (n != 0) {
        avm_error("input expects no arguments " + std::to_string(n));
        retval.type = nil_m;
        return;
    }
    std::string input;
    std::getline(std::cin, input);
    avm_memcellclear(&retval);

    input.erase(0, input.find_first_not_of(" \t\r\n"));
    input.erase(input.find_last_not_of(" \t\r\n") + 1);

    if (input.size() >= 2 && input.front() == '"' && input.back() == '"') {
        retval.type = string_m;
        retval.strVal = input.substr(1, input.length() - 2);
        DEBUG_check("Input string: " + retval.strVal);
        return;
    }
    if (input == "true") {
        retval.type = bool_m;
        retval.boolVal = true;
        DEBUG_check("Input boolean: true");
        return;
    } else if (input == "false") {
        retval.type = bool_m;
        retval.boolVal = false;
        DEBUG_check("Input boolean: false");
        return;
    }
    if (input == "nil") {
        retval.type = nil_m;
        DEBUG_check("Input nil");
        return;
    }

    std::istringstream iss(input);
    double number;
    // flag gia alex
    if (iss >> number && iss.eof()) {
        retval.type = number_m;
        retval.numVal = number;
        DEBUG_check("Input number: " + std::to_string(number));
        return;
    }
}

void libfunc_objectmemberkeys() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("objectmemberkeys expects exactly one argument, not " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }
    avm_memcell* m = avm_getactual(0);
    if (m->type != table_m) {
        avm_error("objectmemberkeys expects a table as argument!");
        retval.type = nil_m;
        return;
    }
    avm_table* table = avm_tablenew();
    // flag gia evi
    // Add keys from number-indexed map --> avm_tablesetelem(new_table, &ax,
    // key);
}
void libfunc_objecttotalmembers() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("objecttotalmembers expects exactly one argument, not " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }
    avm_memcell* arg = avm_getactual(0);
    if (arg->type != table_m) {
        avm_error("objecttotalmembers argument is not a table");
        retval.type = nil_m;
        return;
    }
    avm_table* table = arg->tableVal;
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.numVal = table->total;
}

void libfunc_objectcopy() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("objectcopy expects exactly one argument, not " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg = avm_getactual(0);
    if (arg->type != table_m) {
        avm_error("objectcopy argument is not a table");
        retval.type = nil_m;
        return;
    }

    avm_table* src_table = arg->tableVal;
    avm_table* new_table = avm_tablenew();

    // flag gia evi
    //  Copy number-indexed elements ==> avm_tablemembercopy

    retval.type = table_m;
    retval.tableVal = new_table;
    new_table->total = src_table->total;
    avm_tableincrefcounter(new_table);
}


void libfunc_argument() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("argument expects exactly one argument");
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg0 = avm_getactual(0);
    if (arg0->type != number_m || arg0->numVal < 0) {
        avm_error("argument argument must be a non-negative number");
        retval.type = nil_m;
        return;
    }

    unsigned index = static_cast<unsigned>(arg0->numVal);

    if (topsp == 0) {
        avm_warning("argument called outside of a function");
        retval.type = nil_m;
        return;
    }

    // flag check topsp
    // check total_args
    unsigned total_args = avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);

    if (index >= total_args) {
        avm_warning(
            "argument index out of bounds (i = " + std::to_string(index) +
            ", total = " + std::to_string(total_args) + ")");
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg_cell = &stack[topsp + AVM_STACKENV_SIZE + index + 1];
    avm_memcellclear(&retval);
    retval.type = arg_cell->type;
    switch (arg_cell->type) {
        case number_m:
            retval.numVal = arg_cell->numVal;
            break;
        case string_m:
            retval.strVal = arg_cell->strVal;
            break;
        case bool_m:
            retval.boolVal = arg_cell->boolVal;
            break;
        case table_m:
            retval.tableVal = arg_cell->tableVal;
            break;
        case userfunc_m:
            retval.funcVal = arg_cell->funcVal;
            break;
        case libfunc_m:
            retval.libfuncVal = arg_cell->libfuncVal;
            break;
        default:
            break;
    }
}

void libfunc_strtonum() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("strtonum expects exactly one argument, not " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg = avm_getactual(0);
    if (arg->type != string_m) {
        avm_error("strtonum expects a string as argument!");
        retval.type = nil_m;
        return;
    }
    avm_memcellclear(&retval);
    std::istringstream iss(arg->strVal);
    double number;
    if (iss >> number && iss.eof()) {
        DEBUG_check("strtonum: converting string '" + arg->strVal +
                    "' to number: " + std::to_string(number));
        retval.type = number_m;
        retval.numVal = number;
    } else {
        avm_warning("strtonum: invalid string format, returning 0");
        retval.type = number_m;
        retval.numVal = 0.0;
    }
}
void libfunc_sqrt() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("sqrt expects exactly one argument, not " +
                  std::to_string(n));
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg = avm_getactual(0);
    if (arg->type != number_m || arg->numVal < 0) {
        avm_error("sqrt expects a non-negative number as argument!");
        retval.type = nil_m;
        return;
    }

    double result = std::sqrt(arg->numVal);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.numVal = result;
}

void libfunc_cos() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("cos expects exactly one argument, not " + std::to_string(n));
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg = avm_getactual(0);
    if (arg->type != number_m) {
        avm_error("cos expects a number as argument!");
        retval.type = nil_m;
        return;
    }
    double degrees = arg->numVal;
    degrees = degrees * M_PI / 180.0;
    double result = std::cos(degrees);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.numVal = result;
}

void libfunc_sin() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("sin expects exactly one argument, not " + std::to_string(n));
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg = avm_getactual(0);
    if (arg->type != number_m) {
        avm_error("sin expects a number as argument!");
        retval.type = nil_m;
        return;
    }
    double degrees = arg->numVal;
    degrees = degrees * M_PI / 180.0;
    double result = std::sin(degrees);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.numVal = result;
}

void init_lib_functions() {
    top = AVM_STACKSIZE - 1 - total_glob;
    topsp = AVM_STACKSIZE - 1;
    libFuncs = SymTable_new();
    avm_registerlibfunc("print", libfunc_print);
}
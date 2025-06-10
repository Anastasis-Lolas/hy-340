#include "../avm_execute.h"
#include "../library_functions.h"

// #define DEBUG_check(msg) std::cout << "Reduced: " << msg << " (line " <<
// yylineno << ")\n"
#define DEBUG_check(msg)

extern avm_memcell stack[AVM_STACKSIZE];
SymTable_T libFuncs;

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    switch (func->type) {
        case userfunc_m: {
            avm_callsaveenvironment();  // Maybe outside switch?
            pc = func->data.funcVal;
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            break;
        }
        case string_m:
            avm_callibfunc(func->data.strVal);
            break;
        case libfunc_m:
            avm_callibfunc(func->data.libfuncVal);
            break;
        case table_m:
            avm_call_functor(func->data.tableVal);
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
    cx.data.strVal = "()";

    avm_memcell* f = avm_tablegetelem(table, &cx);
    if (!f)
        avm_error("in calling table: no '()' element found!");
    else if (f->type == table_m) {
        avm_call_functor(f->data.tableVal);
    } else if (f->type == userfunc_m) {
        avm_push_table_arg(table);
        avm_callsaveenvironment();
        pc = f->data.funcVal;
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
    assert(pc == func->data.funcVal); /* Func address should match PC. */

    totalActuals = 0;
    userfunc* f = avm_getfuncinfo(func->data.funcVal);
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
    unsigned val = (unsigned)stack[i].data.numVal;
    assert(stack[i].data.numVal == ((double)val));
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
    retval.data.strVal = typeStrings[avm_getactual(0)->type];
}
void libfunc_totalarguments() {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (!p_topsp) {
        avm_error("totalarguments called outside of a function!");
        retval.type = nil_m;
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
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
            userfunc* f = avm_getfuncinfo(m->data.funcVal);
            if (f) {
                std::cout << "User Function: " << f->id << " at address "
                          << f->address << "\n";
            } else {
                std::cout << "User Function: Unknown at address "
                          << m->data.funcVal << "\n";
            }
        } else if (m->type == libfunc_m) {
            std::cout << "Library Function: " << m->data.libfuncVal << "\n";
        } else if (m->type == table_m) {
            // Assuming avm_table has a toString method or similar flag gia evi
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
        retval.data.strVal = input.substr(1, input.length() - 2);
        DEBUG_check("Input string: " + retval.data.strVal);
        return;
    }
    if (input == "true") {
        retval.type = bool_m;
        retval.data.boolVal = true;
        DEBUG_check("Input boolean: true");
        return;
    } else if (input == "false") {
        retval.type = bool_m;
        retval.data.boolVal = false;
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
        retval.data.numVal = number;
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
    unsigned

        retval.type = table_m;
    retval.data.tableVal = ;
}
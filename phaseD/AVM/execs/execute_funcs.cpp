#include "../../Symtable/symtable.h"
#include "../avm_execute.h"
#include "../library_functions.h"

#define DEBUG_check(msg) std::cout << "[DEBUG]: " << msg << std::endl;
#define DEBUG_colored_red(msg) std::cout << "\033[1;31m[DEBUG]: " << msg << "\033[0m\n";
#define DEBUG_colored_green(msg)std::cout << "\033[1;32m[DEBUG]: " << msg << "\033[0m\n";
// #define DEBUG_check(msg)
// #define DEBUG_colored_green(msg)
// #define DEBUG_colored_red(msg)
// #define DEBUG_colored_red2(msg)
#define DEBUG_colored_red2(msg)std::cout << "\033[1;31m[DEBUG]: " << msg <<"\033[0m\n";


extern avm_memcell stack[AVM_STACKSIZE];
SymTable_T libFuncs;

void execute_call(instruction* instr) {
    DEBUG_check("execute_call");

    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    DEBUG_check("Function to call: " + avm_toString(func));
    assert(func);
    avm_callsaveenvironment();  // Maybe outside switch?
    switch (func->type) {
        case userfunc_m: {
            pc = func->data.funcVal;
            DEBUG_colored_red2(
                "Calling user function at address: " + std::to_string(pc) +
                " | AVM_ENDING_PC: " + std::to_string(AVM_ENDING_PC));
            assert(pc < AVM_ENDING_PC);
            DEBUG_colored_red2("User " + avm_toString(func));
            assert(exec_instructions[pc].opcode == funcenter_v);
            break;
        }
        case string_m:
            avm_callibfunc(func->data.strVal);
            break;
        case libfunc_m:
            DEBUG_check("Calling library function: " + func->data.libfuncVal);
            avm_callibfunc(func->data.libfuncVal);
            break;
        case table_m: {
            DEBUG_check("Attempting to call table as a function.");
            // Check if func->data.tableVal is valid 
            if (!func->data.tableVal) {
                avm_error("Attempt to call a null or uninitialized table!");
                executionFinished = 1;
                break;
            }
            DEBUG_check("Table pointer: " + std::to_string((uintptr_t)func->data.tableVal));
            DEBUG_check("Table refCounter: " + std::to_string(func->data.tableVal->refCounter));
            DEBUG_check("Table total elements: " + std::to_string(func->data.tableVal->total));


            avm_memcell temp_key_cell;
            avm_memcellclear(&temp_key_cell);
            temp_key_cell.type = string_m;
            temp_key_cell.data.strVal = "()";

            //  using key "()"
            avm_memcell* potential_func = avm_tablegetelem(func->data.tableVal, &temp_key_cell);

            avm_memcellclear(&temp_key_cell);
                    if (potential_func) {
                        DEBUG_check("Found '()' in table. Type: " + std::string(typeStrings[potential_func->type]));
                        // Now, recursively call the execution logic for the retrieved function
                        switch (potential_func->type) {
                            case userfunc_m: {
                                pc = potential_func->data.funcVal;
                                DEBUG_colored_red2(
                                    "Calling user function from table at address: " + std::to_string(pc) +
                                    " | AVM_ENDING_PC: " + std::to_string(AVM_ENDING_PC));
                                assert(pc < AVM_ENDING_PC);
                                assert(exec_instructions[pc].opcode == funcenter_v); // 'funcenter_v' or 'enterfunc_v'
                                break;
                            }
                            case string_m:
                                avm_callibfunc(potential_func->data.strVal);
                                break;
                            case libfunc_m:
                                DEBUG_check("Calling library function from table: " + potential_func->data.libfuncVal);
                                avm_callibfunc(potential_func->data.libfuncVal);
                                break;
                            case table_m: {
                                std::string err_msg = "Call: Cannot bind nested table ('()') to function!";
                                avm_error(err_msg);
                                executionFinished = 1;
                                break;
                            }
                            default: {
                                std::string err_msg =
                                    "Call: Cannot bind " + avm_toString(potential_func) + " retrieved from '()' to function!";
                                avm_error(err_msg);
                                executionFinished = 1;
                                break;
                            }
                        }       
                } else {
                std::string err_msg = "Call: Table does not contain a '()' function!";
                avm_error(err_msg);
                executionFinished = 1;
            }
            break;
        }
        default:
            std::string err_msg =
                "Call: cannot bind " + avm_toString(func) + " to function!";
            avm_error(err_msg);
            executionFinished = 1;
    }
    DEBUG_check("=====================\tENDexecute_call");
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
        // avm_callsaveenvironment();
        pc = f->data.funcVal;
        // pc < AVM_ENDING_PC &&
        assert(code[pc].opcode == funcenter_v);
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
        // avm_callsaveenvironment();
        topsp = top;
        totalActuals = 0;
        (*f)();
        if (!executionFinished) {
            execute_funcexit((instruction*)0);
        }
    }
}

void execute_pusharg(instruction* instr) {
    DEBUG_colored_red("execute_pusharg" << " | arg1: " << instr->arg1.type
                                        << ", val: " << instr->arg1.val
                                        << " | result: " << instr->result.type
                                        << ", val: " << instr->result.val);
    // if (instr->arg1.val == 2) instr->arg1.val = 1;
    avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
    DEBUG_colored_red("Argument to push: " + avm_toString(arg));

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
    DEBUG_colored_red2(("execute_funcenter: " + std::string(" | local size: ") +
                        std::to_string(f->localSize)));
}






// void execute_funcenter(instruction* instr) {
//     avm_memcell* func = avm_translate_operand(&instr->result, &ax);
//     assert(func);
//     assert(pc == func->data.funcVal); /* Func address should match PC. */

//     // Save totalActuals *before* resetting
//     unsigned int numActuals = totalActuals;
//     totalActuals = 0; // Reset for the callee's environment

//     userfunc* f = avm_getfuncinfo(func->data.funcVal);

//     topsp = top; 

//     top = top - f->localSize; 
//     DEBUG_colored_red2(("execute_funcenter: Setting topsp: " + std::to_string(topsp) +
//                         " | New top: " + std::to_string(top) +
//                         " | local size: " + std::to_string(f->localSize) +
//                         " | Actual args to copy: " + std::to_string(numActuals)));
   

//     for (unsigned int i = 0; i < numActuals; ++i) {
       
//         avm_memcell* src_arg_cell = &stack[topsp + 4 + i];

        
//         avm_memcell* dest_formal_cell = &stack[topsp - numActuals + i];

//         avm_memcellclear(dest_formal_cell);
//         avm_assign(dest_formal_cell, src_arg_cell);

//         DEBUG_colored_red2("Copied Arg " + std::to_string(i) + " (" + avm_toString(src_arg_cell) + ") to formal slot " + std::to_string(topsp - numActuals + i));
//     }
// }


void execute_funcexit(instruction*) {
    int oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    while (++oldTop < top) { /* Intentionally ignoring first. */
        DEBUG_colored_green("Clearing stack cell at index " +
                            std::to_string(oldTop));
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
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
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
    DEBUG_colored_red(
        "libfunc_typeof: " + avm_toString(avm_getactual(0)) +
        " | type: " + typeStrings[avm_getactual(0)->type] +
        " | to string: " + memcell_type_to_string(avm_getactual(0)->type));
    // new (&retval.data.strVal)
    // std::string(typeStrings[avm_getactual(0)->type]);
    // new (&retval.data.strVal)
    //     std::string(memcell_type_to_string(avm_getactual(0)->type));
    new (&retval.data.strVal) std::string(typeStrings[avm_getactual(0)->type]);
    // retval.data.strVal = typeStrings[avm_getactual(0)->type];
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
                          << f->address;
            } else {
                std::cout << "User Function: Unknown at address "
                          << m->data.funcVal;
            }
        } else if (m->type == libfunc_m) {
            std::cout << "Library Function: " << m->data.libfuncVal;
        } else if (m->type == table_m) {
            std::cout << table_toString(m);
            // assert(0);
        } else {
            std::cout << avm_toString(m);
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
        new (&retval.data.strVal)
            std::string(input.substr(1, input.length() - 2));

        // retval.data.strVal = input.substr(1, input.length() - 2);
        return;
    }
    if (input == "true") {
        retval.type = bool_m;
        retval.data.boolVal = true;
        return;
    } else if (input == "false") {
        retval.type = bool_m;
        retval.data.boolVal = false;
        return;
    }
    if (input == "nil") {
        retval.type = nil_m;
        return;
    }

    std::istringstream iss(input);
    double number;
    // flag gia alex
    if (iss >> number && iss.eof()) {
        retval.type = number_m;
        retval.data.numVal = number;
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
    // flag gia evi
    avm_memcellclear(&retval);
    retval.type = table_m;
    retval.data.tableVal = avm_tablenew();
    avm_tableincrefcounter(retval.data.tableVal);

    // pointers to the source and destination tables
    avm_table* src_table = m->data.tableVal;
    avm_table* keys_table = retval.data.tableVal;
    unsigned int counter = 0;
    // iterate through all key-types of the source table
    for (auto const& [key, val] : *src_table->numIndexed) {
        avm_memcell new_index, new_content;

        new_index.type = number_m;
        new_index.data.numVal = counter++;

        new_content.type = number_m;
        new_content.data.numVal = key;

        avm_tablesetelem(keys_table, &new_index, &new_content);
    }
    // ηandle string keys
    for (auto const& [key, val] : *src_table->strIndexed) {
        avm_memcell new_index, new_content;
        new_index.type = number_m;
        new_index.data.numVal = counter++;
        new_content.type = string_m;
        new (&new_content.data.strVal) std::string(key);

        avm_tablesetelem(keys_table, &new_index, &new_content);

        new_content.data.strVal.~basic_string();
    }
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
    avm_table* table = arg->data.tableVal;
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.data.numVal = table->total;
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

    avm_table* src_table = arg->data.tableVal;
    avm_table* new_table = avm_tablenew();

    // flag gia evi
    //  Copy number-indexed elements ==> avm_tablemembercopy

    // handle arithmetic keys
    for (auto const& [key, val] : *src_table->numIndexed) {
        avm_assign(&(*new_table->numIndexed)[key],
                   const_cast<avm_memcell*>(&val));
    }
    // handle string keys
    for (auto const& [key, val] : *src_table->strIndexed) {
        avm_assign(&(*new_table->strIndexed)[key],
                   const_cast<avm_memcell*>(&val));
    }
    new_table->total = src_table->total;
    retval.type = table_m;
    retval.data.tableVal = new_table;
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
    if (arg0->type != number_m || arg0->data.numVal < 0) {
        avm_error("argument argument must be a non-negative number");
        retval.type = nil_m;
        return;
    }
    unsigned index = static_cast<unsigned>(arg0->data.numVal);
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    if (p_topsp == 0) {
        avm_warning("argument called outside of a function");
        retval.type = nil_m;
        return;
    }
    unsigned total_args = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);

    if (index >= total_args) {
        avm_warning(
            "argument index out of bounds (i = " + std::to_string(index) +
            ", total = " + std::to_string(total_args) + ")");
        retval.type = nil_m;
        return;
    }

    avm_memcell* arg_cell = &stack[p_topsp + AVM_STACKENV_SIZE + 1 + index];

    avm_memcellclear(&retval);
    retval.type = arg_cell->type;
    switch (arg_cell->type) {
        case number_m:
            retval.data.numVal = arg_cell->data.numVal;
            break;
        case string_m:
            new (&retval.data.strVal) std::string(arg_cell->data.strVal);
            break;
        case bool_m:
            retval.data.boolVal = arg_cell->data.boolVal;
            break;
        case table_m:
            retval.data.tableVal = arg_cell->data.tableVal;
            avm_tableincrefcounter(
                retval.data.tableVal);  // Πρόσθεσε αυτό για σωστό ref counting
            break;
        case userfunc_m:
            retval.data.funcVal = arg_cell->data.funcVal;
            break;
        case libfunc_m:
            new (&retval.data.libfuncVal)
                std::string(arg_cell->data.libfuncVal);
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
    std::istringstream iss(arg->data.strVal);
    double number;
    if (iss >> number && iss.eof()) {
        retval.type = number_m;
        retval.data.numVal = number;
    } else {
        avm_warning("strtonum: invalid string format, returning 0");
        retval.type = nil_m;
        // retval.data.numVal = 0.0;
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
    DEBUG_colored_red2(
        "libfunc_sqrt: arg type: " + memcell_type_to_string(arg->type) +
        ", value: " + avm_toString(arg) +
        " | arg->data.numVal: " + std::to_string(arg->data.numVal));
    if (arg->type != number_m || arg->data.numVal < 0) {
        avm_error("sqrt expects a non-negative number as argument!");
        retval.type = nil_m;
        return;
    }

    double result = std::sqrt(arg->data.numVal);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.data.numVal = result;
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
    double degrees = arg->data.numVal;
    degrees = degrees * M_PI / 180.0;
    double result = std::cos(degrees);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.data.numVal = result;
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
    double degrees = arg->data.numVal;
    degrees = degrees * M_PI / 180.0;
    double result = std::sin(degrees);
    avm_memcellclear(&retval);
    retval.type = number_m;
    retval.data.numVal = result;
}

void init_lib_functions() {
    top = AVM_STACKSIZE - 1 - total_glob;
    topsp = AVM_STACKSIZE - 1;
    libFuncs = SymTable_new();
    avm_registerlibfunc("print", libfunc_print);
    avm_registerlibfunc("input", libfunc_input);
    avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
    avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
    avm_registerlibfunc("objectcopy", libfunc_objectcopy);
    avm_registerlibfunc("totalarguments", libfunc_totalarguments);
    avm_registerlibfunc("argument", libfunc_argument);
    avm_registerlibfunc("typeof", libfunc_typeof);
    avm_registerlibfunc("strtonum", libfunc_strtonum);
    avm_registerlibfunc("sqrt", libfunc_sqrt);
    avm_registerlibfunc("cos", libfunc_cos);
    avm_registerlibfunc("sin", libfunc_sin);
}
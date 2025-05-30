#include "../avm_execute.h"
#include "../avm_helper.h"

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

void execute_pusharg(instruction*) {}
void execute_funcenter(instruction*) {}
void execute_funcexit(instruction*) {}

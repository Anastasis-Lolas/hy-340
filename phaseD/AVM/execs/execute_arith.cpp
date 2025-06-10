#include "../avm_execute.h"


extern avm_memcell stack[AVM_STACKSIZE];

arithmetic_func_t arithmeticFuncs[] = {add_impl, sub_impl, mul_impl, div_impl,
                                       mod_impl};


void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[N - 1] >= lv && lv > &stack[top] || lv == &retval));
    assert(rv);  // should do similar assertion tests here

    avm_assign(lv, rv);
}


void avm_assign(avm_memcell* lv, avm_memcell* rv) {
    if (lv == rv) return;

    if (lv->type == table_m && rv->type == table_m &&
        lv->data.tableVal == rv->data.tableVal)
        return;

    if (rv->type == undef_m) {
        avm_warning("assigning from 'undef' content!");

    }
    avm_memcellclear(lv);

    memcpy(lv, rv, sizeof(avm_memcell));


    if (lv->type == string_m)
        //lv->data.strVal = strdup(rv->data.strVal);
        lv->data.strVal = rv->data.strVal;
    else if (lv->type == table_m)
        avm_tableincrefcounter(lv->data.tableVal);
}

double add_impl(double x, double y) { return x + y; }
double sub_impl(double x, double y) { return x - y; }
double mul_impl(double x, double y) { return x * y; }
double div_impl(double x, double y) {
    if (y != 0) {
        return x / y;
    } else {
        std::cout << "DIV WITH 0 !\n";
        return -1;
    }
}
double mod_impl(double x, double y) {
    return ((unsigned)x) % ((unsigned)y); /* Error check */
}

void execute_arithmetic(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[N - 1] >= lv && lv > &stack[top] || lv == &retval));
    assert(rv1 && rv2);

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("not a number in arithmetic!");
        executionFinished = 1;
    } else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}

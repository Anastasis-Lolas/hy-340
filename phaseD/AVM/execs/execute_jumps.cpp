#include "../avm_execute.h"


char* typeStrings[] = {"number",   "string",  "bool", "table",
                       "userfunc", "libfunc", "nil",  "undef"};

equality_check equal_check_dispatcher[] = {check_eq_number, check_eq_strings};

jump_cmp_func jump_dispatcher[] = {jge_impl, jgt_impl, jle_impl, jlt_impl};

double jge_impl(double x, double y) { return x >= y; }
double jgt_impl(double x, double y) { return x > y; }
double jle_impl(double x, double y) { return x <= y; }
double jlt_impl(double x, double y) { return x < y; }

void execute_rljmp(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char comparison_result_bool = 0;

    if (executionFinished) {
        return;
    }

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("Operands for relational jump must be numbers.");
        executionFinished = 1;
    } else {
        unsigned int dispatcher_idx = instr->opcode - jge_v;

        if (dispatcher_idx <
            (sizeof(jump_dispatcher) / sizeof(jump_cmp_func))) {
            double comparison_value = jump_dispatcher[dispatcher_idx](
                rv1->data.numVal, rv2->data.numVal);
            comparison_result_bool = (comparison_value != 0.0);
        } else {
            avm_error(
                "Internal error: Relational jump opcode out of dispatcher "
                "bounds.");
            executionFinished = 1;
        }
    }

    if (!executionFinished && comparison_result_bool) {
        pc = instr->result.val;
    }
}

unsigned char check_eq_number(avm_memcell* op1, avm_memcell* op2) {
    return (op1->data.numVal == op2->data.numVal);
}
unsigned char check_eq_strings(avm_memcell* op1, avm_memcell* op2) {
    return (op1->data.strVal == op2->data.strVal);
}

void execute_jump(instruction* instr) {
    assert(instr->result.type = label_a);

    pc = instr->result.val;
}


void execute_jeq(instruction* instr) {
    assert(instr->result.type == label_a);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("'undef' involved in equality!");
    else if (rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    else if (rv1->type != rv2->type)
        avm_error("%s is illegal!", typeStrings[rv1->type],
                  typeStrings[rv2->type]);
    else {
        /* Equality check with dispatching. */
        result = equal_check_dispatcher[rv1->type](rv1, rv2);
    }

    if (!executionFinished && result) pc = instr->result.val;
}


void execute_jne(instruction*) void execute_jle(instruction*) void execute_jge(
    instruction*) void execute_jlt(instruction*) void execute_jgt(instruction*)
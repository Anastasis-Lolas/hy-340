#include "../avm_execute.h"


extern avm_memcell stack[AVM_STACKSIZE];

equality_check equal_check_dispatcher[] = {check_eq_number, check_eq_strings};

jump_cmp_func jump_dispatcher[] = {jge_impl, jgt_impl, jle_impl, jlt_impl};

double jge_impl(double x, double y) { return x >= y; }
double jgt_impl(double x, double y) { return x > y; }
double jle_impl(double x, double y) { return x <= y; }
double jlt_impl(double x, double y) { return x < y; }

void execute_rljump(instruction* instr) {
    // std::cout << "[AVM DEBUG] relational!\n";
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (executionFinished) {
        return;
    }

    if (rv1->type == bool_m || rv2->type == bool_m) {
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    } else if (rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;
    else if (rv1->type != rv2->type)
        avm_error("Assigning diff types is illegal");
    else {
        // std::cout << "[AVM DEBUG] Comparing: (" << rv1->data.numVal
        //           << ") with (" << rv2->data.numVal << ")" << std::endl;
        switch (instr->opcode) {
            case jge_v:
                // std::cout << "[AVM DEBUG] jge \n";
                result = jge_impl(rv1->data.numVal, rv2->data.numVal);
                break;
            case jgt_v:
                // std::cout << "[AVM DEBUG] jgt \n";
                result = jgt_impl(rv1->data.numVal, rv2->data.numVal);
                break;
            case jle_v:
                // std::cout << "[AVM DEBUG] jle \n";
                result = jle_impl(rv1->data.numVal, rv2->data.numVal);
                break;
            case jlt_v:
                // std::cout << "[AVM DEBUG] jlt \n";
                result = jlt_impl(rv1->data.numVal, rv2->data.numVal);
                break;
            default:
                avm_error("Internal error: Unhandled relational jump opcode.");
                executionFinished = 1;
                break;
        }
    }

    if (!executionFinished && result) {
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
    assert(instr->result.type == label_a);
    unsigned int target_pc = instr->result.val;
    // std::cout << "[AVM DEBUG] JUMP to instruction: " << target_pc <<
    // std::endl;
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
        avm_error(typeStrings[rv1->type] + std::string(" ") +
                  typeStrings[rv2->type] + " is illegal!");


    else {
        /* Equality check with dispatching. */
        result = equal_check_dispatcher[rv1->type](rv1, rv2);
    }

    if (!executionFinished && result) pc = instr->result.val;
}


void execute_nop(instruction*) {
    // No operation, just a placeholder
    std::cout << "!!! executed nop! !!!\n";
    assert(0);
}


void execute_jne(instruction* instr) {
    assert(instr->result.type == label_a);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("'undef' involved in equality!");

    else if (rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;

    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) != avm_tobool(rv2));

    else if (rv1->type != rv2->type)
        avm_error(typeStrings[rv1->type] + std::string(" ") +
                  typeStrings[rv2->type] + " is illegal!");
    else {
        unsigned char equal = (*equal_check_dispatcher[rv1->type])(rv1, rv2);
        result = !equal;
        ;
    }

    if (!executionFinished && result) pc = instr->result.val;
}
void execute_jle(instruction*);
void execute_jge(instruction*);
void execute_jlt(instruction*);
void execute_jgt(instruction*);
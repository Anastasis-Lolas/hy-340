#include "../avm_execute.h"
#include "../avm_helper.h"
#include "../avm_table.h"


extern avm_memcell stack[AVM_STACKSIZE];


void execute_newtable(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*)0);
    assert(lv && (&stack[N - 1] >= lv && lv > &stack[top] || lv == &retval));

    avm_memcellclear(lv);

    lv->type = table_m;
    lv->tableVal = avm_tablenew();
    avm_tableincrefcounter(lv->tableVal);
}


void execute_tablegetelem(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* t = avm_translate_operand(&instr->arg1, (avm_memcell*)0);
    avm_memcell* i = avm_translate_operand(&instr->arg2, &ax);

    assert(lv && &stack[N - 1] >= lv && lv > &stack[top] || lv == &retval);
    assert(t && &stack[N - 1] >= t && t > &stack[top]);
    assert(i);

    avm_memcellclear(lv);
    lv->type = nil_m; /* Default value */

    if (t->type != table_m)
        avm_error("Illegal use of type " + typeStrings[t->type] + " as table!");

    else {
        avm_memcell* content = avm_tablegetelem(t->tableVal, i);
        if (content)
            avm_assign(lv, content);
        else {
            std::string ts = avm_toString(t);
            std::string is = avm_toString(i);
            avm_warning(ts + "[" + is + "] not found!");
        }
    }
}
void execute_tablesetelem(instruction* instr) {
    avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);

    assert(t && &stack[N - 1] >= t && t > &stack[top]);
    assert(i && c);

    if (t->type != table_m)
        avm_error("Illegal use of type " + typeStrings[t->type] + " as table!");
    else
        avm_tablesetelem(t->tableVal, i, c);
}

void avm_push_table_arg(avm_table* t) {
    stack[top].type = table_m;
    avm_tableincrefcounter(stack[top].tableVal = t);
    ++totalActuals;
    avm_dec_top();
}
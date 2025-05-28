#include "t-code.h"

std::vector<incomplete_jump *> incjumps_vec;
std::vector<std::string> string_vec_consts;
std::vector<double> double_vec_consts;
std::vector<std::string> lig_strvec_consts;
std::vector<int> int_vec_consts;

std::vector<SymbolTableEntry_T> newfunc_vec_consts;
extern std::vector<quad *> quad_table;

std::vector<instruction *> instruction_table;
unsigned int currInst = 0;

generator_func_t generators[] = {
    generate_ADD,          generate_SUB,       generate_DIV,
    generate_MOD,          generate_NEWTABLE,  generate_TABLEGETELEM,
    generate_ASSIGN,       generate_NOP,       generate_JUMP,
    generate_IF_EQ,        generate_IF_NOTEQ,  generate_IF_GREATER,
    generate_IF_GREATEREQ, generate_IF_LESS,   generate_IF_LESSEQ,
    generate_NOT,          generate_PARAM,     generate_CALL,
    generate_GETRETVAL,    generate_FUNCSTART, generate_RETURN,
    generate_FUNCEND

};

unsigned consts_newstring(std::string s) {
    for (unsigned int i = 0; i < string_vec_consts.size(); ++i) {
        if (string_vec_consts[i] == s) {
            return i;
        }
    }
    string_vec_consts.push_back(s);
    return string_vec_consts.size() - 1;
}

unsigned consts_newint(int a) {
    for (unsigned int i = 0; i < int_vec_consts.size(); ++i) {
        if (int_vec_consts[i] == a) {
            return i;
        }
    }
    int_vec_consts.push_back(a);
    return int_vec_consts.size() - 1;
}

unsigned consts_newdouble(double n) {
    for (unsigned int i = 0; i < double_vec_consts.size(); ++i) {
        if (double_vec_consts[i] == n) {
            return i;
        }
    }
    double_vec_consts.push_back(n);
    return double_vec_consts.size() - 1;
}

unsigned libfuncs_newused(std::string s) {
    for (unsigned int i = 0; i < lig_strvec_consts.size(); ++i) {
        if (lig_strvec_consts[i] == s) {
            return i;
        }
    }
    lig_strvec_consts.push_back(s);
    return lig_strvec_consts.size() - 1;
}

unsigned userfunc_newfunc(SymbolTableEntry_T sym) {
    for (int i = 0; i < newfunc_vec_consts.size(); i++) {
        if (newfunc_vec_consts[i] == sym) {
            return i;
        }
    }

    newfunc_vec_consts.push_back(sym);
    return newfunc_vec_consts.size() - 1;
}

void make_operand(expr *e, vmarg *arg) {
    switch (e->type) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e: {
            assert(e->sym);
            arg->val = e->sym->value.varVal->offset;
            switch (e->sym->type) {
                case GLOBAL:
                    arg->type = global_a;
                    break;
                case LLOCAL:
                    arg->type = local_a;
                    break;
                case FORMAL:
                    arg->type = formal_a;
                    break;
                default:
                    assert(0);
            }
            break;
        }
        case constbool_e: {
            arg->val = e->boolConst;
            arg->type = bool_a;
            break;
        }
        case conststring_e: {
            arg->val = consts_newstring(e->strConst);
            arg->type = string_a;
            break;
        }

        case constnum_e: {
            arg->val = consts_newdouble(e->numConst);
            arg->type = number_a;
            break;
        }
        case nil_e:
            arg->type = nil_a;
            break;
        case programfunc_e: {
            arg->val = userfunc_newfunc(e->sym);
            arg->type = userfunc_a;
            break;
        }
        case libraryfunc_e: {
            arg->val = libfuncs_newused(e->sym->value.funcVal->name);
            arg->type = libfunc_a;
            break;
        }
        default:
            assert(0);
    }
}


void vm_emit(instruction *t) {
    instruction *new_inst = (instruction *)malloc(sizeof(instruction));

    if (!new_inst) {
        std::cerr << "Failed !" << std::endl;
        return;
    }

    new_inst->opcode = t->opcode;
    new_inst->arg1 = t->arg1;
    new_inst->arg2 = t->arg2;
    new_inst->result = t->result;
    new_inst->srcLine = t->srcLine;
    instruction_table.push_back(new_inst);

    currInst++;
}


void add_incomplete_jump(unsigned instrNo, unsigned iaddress) {
    incomplete_jump *new_inc =
        (incomplete_jump *)malloc(sizeof(incomplete_jump));

    new_inc->instrNo = instrNo;
    new_inc->iaddress = iaddress;

    incjumps_vec.push_back(new_inc);
    ij_total++;
}

void make_numberoperand(vmarg *arg, double val) {
    arg->val = consts_newdouble(val);
    arg->type = number_a;
}

void make_booloperand(vmarg *arg, unsigned val) {
    arg->val = val;
    arg->type = bool_a;
}

void make_retvaloperand(vmarg *arg) { arg->type = retval_a; }

void reset_operand(vmarg *arg) {
    arg->val = 0;
    arg->type = undef_a;
}

unsigned nextinstructionlabel() { return currInst; }

void generate(vmopcode op, quad *quad) {
    instruction *t = (instruction *)malloc(sizeof(instruction));
    t->opcode = op;

    if (quad->arg1) make_operand(quad->arg1, t->arg1);

    if (quad->arg2) make_operand(quad->arg2, t->arg2);

    if (quad->result) make_operand(quad->result, t->result);

    t->srcLine = quad->line;
    quad->taddress = nextinstructionlabel();

    vm_emit(t);
}

void generate_relational(vmopcode op, quad *q) {
    instruction *t = (instruction *)malloc(sizeof(instruction));
    t->opcode = op;

    if (q->arg1) make_operand(q->arg1, t->arg1);

    if (q->arg2) make_operand(q->arg2, t->arg2);

    t->result->type = label_a;

    if (q->label < nextquad()) {
        t->result->val = quad_table[q->label]->taddress;
    } else {
        add_incomplete_jump(nextinstructionlabel(), q->label);
    }

    q->taddress = nextinstructionlabel();
    vm_emit(t);
}


void generate_ADD(quad *q) { generate(add_v, q); }
void generate_SUB(quad *q) { generate(sub_v, q); }
void generate_MUL(quad *q) { generate(mul_v, q); }
void generate_DIV(quad *q) { generate(div_v, q); }
void generate_MOD(quad *q) { generate(mod_v, q); }
void generate_NEWTABLE(quad *q) { generate(newtable_v, q); }
void generate_TABLEGETELEM(quad *q) { generate(tablegetelem_v, q); }
void generate_TABLESETELEM(quad *q) { generate(tablesetelem_v, q); }
void generate_ASSIGN(quad *q) { generate(assign_v, q); }
void generate_NOP(quad *) {
    instruction *t = (instruction *)malloc(sizeof(instruction));
    t->opcode = not_v;
    // ??
    t->arg1 = nullptr;
    t->arg2 = nullptr;
    t->result = nullptr;

    vm_emit(t);
}


void generate_JUMP(quad *q) { generate_relational(jump_v, q); }
void generate_IF_EQ(quad *q) { generate_relational(jeq_v, q); }
void generate_IF_NOTEQ(quad *q) { generate_relational(jne_v, q); }
void generate_IF_GREATER(quad *q) { generate_relational(jgt_v, q); }
void generate_IF_GREATER_EQ(quad *q) { generate_relational(jge_v, q); }
void generate_IF_LESS(quad *q) { generate_relational(jlt_v, q); }
void generate_IF_LESSEQ(quad *q) { generate_relational(jle_v, q); }


void generate_NOT(quad *q) {
    q->taddress = nextinstructionlabel();

    instruction *t = new instruction();

    t->opcode = jeq_v;

    if (q->arg1) make_operand(q->arg1, t->arg1);

    make_booloperand(t->arg2, false);

    t->result->type = label_a;

    t->result->val = nextinstructionlabel() + 3;

    vm_emit(t);


    t->opcode = assign_v;

    make_booloperand(t->arg1, false);

    if (t->arg2) reset_operand(t->arg2);

    if (q->result) make_operand(q->result, t->result);

    vm_emit(t);

    t->opcode = jump_v;

    if (t->arg1) reset_operand(t->arg1);

    if (t->arg2) reset_operand(t->arg2);

    t->result->type = label_a;
    t->result->val = nextinstructionlabel() + 2;

    vm_emit(t);

    t->opcode = assign_v;

    if (t->arg1) make_booloperand(t->arg1, true);

    if (t->arg2) reset_operand(t->arg2);

    if (q->result) make_operand(q->result, t->result);

    vm_emit(t);
}


void generate_OR(quad *q) {
    q->taddress = nextinstructionlabel();

    instruction *t = new instruction();

    t->opcode = jeq_v;

    if (q->arg1) make_operand(q->arg1, t->arg1);

    make_booloperand(t->arg2, true);

    t->result->type = label_a;
    t->result->val = nextinstructionlabel() + 4;
    vm_emit(t);

    if (q->arg2) make_operand(q->arg2, t->arg1);

    t->result->val = nextinstructionlabel() + 3;
    vm_emit(t);

    t->opcode = assign_v;
    make_booloperand(t->arg1, false);

    if (t->arg2) reset_operand(t->arg2);

    if (q->result) make_operand(q->result, t->result);

    vm_emit(t);

    t->opcode = assign_v;
    make_booloperand(t->arg1, true);

    if (t->arg2) reset_operand(t->arg2);

    if (q->result) make_operand(q->result, t->result);

    vm_emit(t);
}

void generate_PARAM(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = pusharg_v;

    if (q->arg1) make_operand(q->arg1, t->arg1);

    vm_emit(t);
}

void generate_CALL(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = call_v;

    if (q->arg1) make_operand(q->arg1, t->arg1);

    vm_emit(t);
}

void generate_GETRETVAL(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();

    t->opcode = assign_v;

    if (q->result) make_operand(q->result, t->result);
    make_retvaloperand(t->arg1);

    vm_emit(t);
}

void generate_FUNCEND(quad *q) {
    // SymTableEntry f = pop(funcstack);
    // backpatch(f.returnList, nextinstructionlabel());
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = funcexit_v;
    make_operand(q->result, t->result);

    vm_emit(t);
}

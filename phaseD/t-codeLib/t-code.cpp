#include "t-code.h"

std::vector<incomplete_jump *> incjumps_vec;
std::vector<std::string> string_vec_consts;
std::vector<double> double_vec_consts;
std::vector<std::string> lig_strvec_consts;
std::vector<int> int_vec_consts;
std::vector<userfunc> funcstack;
std::vector<int> labstack;
extern std::vector<quad *> quad_table;

std::vector<instruction *> instruction_table;
unsigned int currInst = 0;

incomplete_jump *ij_head = (incomplete_jump *)0;
unsigned ij_total = 0;


generator_func_t generators[] = {
    generate_ADD,       generate_SUB,          generate_DIV,
    generate_MOD,       generate_ASSIGN,       generate_UMINUS,
    generate_NOP,       generate_AND,          generate_OR,
    generate_NOT,       generate_IF_EQ,        generate_IF_NOTEQ,
    generate_IF_LESS,   generate_IF_GREATER,
    generate_IF_GREATEREQ,   generate_CALL,    generate_IF_LESSEQ,
    generate_PARAM,     generate_RETURN,       generate_GETRETVAL,
    generate_FUNCSTART, generate_FUNCEND,      generate_NEWTABLE,
    generate_JUMP,      generate_TABLEGETELEM, generate_TABLESETELEM};

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
                    printf("Error1: Invalid symbol type for operand\n");
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

        case constdouble_e: {
            arg->val = consts_newdouble(e->numConst);
            arg->type = number_a;
            break;
        }
        case intnum_e: {
            arg->val = consts_newint(e->numInt);
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
            std::cerr << "Error: Invalid symbol type for operand\n";
            assert(0);
    }
}


void vm_emit(instruction *t) {
    instruction *new_inst = new instruction();
    new_inst->arg1 = new vmarg();
    new_inst->arg2 = new vmarg();
    new_inst->result = new vmarg();

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
}

void patch_incomplete_jumps() {
    for (unsigned i = 0; i < incjumps_vec.size(); i++) {
        if (incjumps_vec[i]->iaddress == nextquadlabel()) {
            instruction_table[incjumps_vec[i]->instrNo]->result->val =
                nextinstructionlabel();
        } else {
            instruction_table[incjumps_vec[i]->instrNo]->result->val =
                quad_table[incjumps_vec[i]->iaddress]->taddress;
        }
    }
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
    instruction *t = new instruction();
    t->opcode = op;

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

    if (quad->arg1) make_operand(quad->arg1, t->arg1);

    if (quad->arg2) make_operand(quad->arg2, t->arg2);

    if (quad->result) make_operand(quad->result, t->result);

    t->srcLine = quad->line;
    quad->taddress = nextinstructionlabel();

    vm_emit(t);
}

std::string vmopcode_to_string(vmopcode op) {
    switch (op) {
        case assign_v:
            return "assign";
        case add_v:
            return "add";
        case sub_v:
            return "sub";
        case mul_v:
            return "mul";
        case div_v:
            return "div";
        case mod_v:
            return "mod";
        case uminus_v:
            return "uminus";
        case and_v:
            return "and";
        case or_v:
            return "or";
        case not_v:
            return "not";
        case jeq_v:
            return "jeq";
        case jne_v:
            return "jne";
        case jle_v:
            return "jle";
        case jge_v:
            return "jge";
        case jlt_v:
            return "jlt";
        case jgt_v:
            return "jgt";
        case call_v:
            return "call";
        case pusharg_v:
            return "pusharg";
        case funcenter_v:
            return "funcenter";
        case funcexit_v:
            return "funcexit";
        case newtable_v:
            return "newtable";
        case tablegetelem_v:
            return "tablegetelem";
        case tablesetelem_v:
            return "tablesetelem";
        case jump_v:
            return "jump";
        case ret_v:
            return "ret";
        case nop_v:
            return "nop";
        default:
            return "unknown_op";
    }
}

void generate_relational(vmopcode op, quad *q) {
    instruction *t = new instruction();
    t->opcode = op;

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

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
void generate_UMINUS(quad *q) { 
    q->taddress = nextinstructionlabel();
    instruction * instr  = new instruction();
    instr->opcode = mul_v;
    instr->srcLine = q->line;

    // arg1 = -1
    instr->arg1 = new vmarg();
    make_numberoperand(instr->arg1, -1.0);


    instr->arg2 = new vmarg();
    make_operand(q->arg1, instr->arg2);


    instr->result = new vmarg();
    make_operand(q->result, instr->result);

    vm_emit(instr);}

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
void generate_IF_GREATEREQ(quad *q) { generate_relational(jge_v, q); }

void generate_IF_LESS(quad *q) { generate_relational(jlt_v, q); }

void generate_IF_LESSEQ(quad *q) { generate_relational(jle_v, q); }


void generate_NOT(quad *q) {
    q->taddress = nextinstructionlabel();

    instruction *t = new instruction();

    t->opcode = jeq_v;

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

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

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

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

void generate_AND(quad *q) {
    q->taddress = nextinstructionlabel();

    instruction *t = new instruction();

    t->opcode = jeq_v;

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

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

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();

    if (q->arg1) make_operand(q->arg1, t->arg1);

    vm_emit(t);
}

void generate_CALL(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = call_v;

    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();
    if (q->result) make_operand(q->result, t->result);

    vm_emit(t);
}

void generate_GETRETVAL(quad *q) {
    assert(q);

    instruction *t = new instruction();
    t->opcode = assign_v;
    q->taddress = nextinstructionlabel();
    t->srcLine = q->line;  // ? q->taddress;
    t->arg1 = new vmarg();

    if (q->result) {
        t->result = new vmarg();
        make_operand(q->result, t->result);
    } else {
        t->result = nullptr;
    }

    make_retvaloperand(t->arg1);

    vm_emit(t);
}

void generate_RETURN(quad *q) {
    assert(q);
    instruction *t = new instruction();
    t->opcode = assign_v;
    q->taddress = nextinstructionlabel();  // ?
    t->srcLine = q->line;                  // ? q->taddress;

    if (q->result) {
        t->arg1 = new vmarg();
        make_operand(q->result, t->arg1);
    } else {
        t->arg1 = nullptr;
    }

    t->arg2 = nullptr;
    t->result = nullptr;

    vm_emit(t);
}


void generate_FUNCEND(quad *q) {
    // SymTableEntry f = pop(funcstack);
    // backpatch(f.returnList, nextinstructionlabel());
    assert(q);

    q->taddress = nextinstructionlabel();  // ?

    instruction *t = new instruction();
    t->opcode = funcexit_v;
    t->srcLine = q->line;  // ? q->taddress;
    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();
    if (q->result) make_operand(q->result, t->result);
    t->result = new vmarg();
    t->result->type = label_a;
    t->result->val = labstack.back();
    labstack.pop_back();
    vm_emit(t);
}

void generate_FUNCSTART(quad *q) {
    assert(q);
    SymbolTableEntry_T f = q->result->sym;

    // f->taddress = nextinstructionlabel(); // ?
    q->taddress = nextinstructionlabel();  // ?
    instruction *t = new instruction();
    t->opcode = funcenter_v;
    t->srcLine = q->line;  // ? q->taddress;
    t->arg1 = new vmarg();
    t->arg2 = new vmarg();
    t->result = new vmarg();
    if (q->result) make_operand(q->result, t->result);
    labstack.push_back(funcstack.size() - 1);

    vm_emit(t);
}




unsigned userfunc_newfunc(SymbolTableEntry_T sym) {
    assert(sym);
    assert(sym->type == USERFUNC);
    for (unsigned int i = 0; i < funcstack.size(); ++i) {
        if (funcstack[i].id == sym->value.funcVal->name) {
            return i;
        }
    }
    userfunc *f = new userfunc();
    f->address = sym->value.funcVal->funcIndex;
    f->localSize = sym->value.funcVal->totalLocals;
    f->id = sym->value.funcVal->name;
    funcstack.push_back(*f);
    return funcstack.size() - 1;
}


// void generate_instructions() {
//     for (unsigned int i = 0; i < quad_table.size(); ++i) {
//         quad *q = quad_table[i];
//         if (!q) continue;
//         generators[q->op](q);
//     }
// }
void generate_instructions() {
    for (unsigned int i = 0; i < quad_table.size(); ++i) {
        quad *q = quad_table[i];
        if (!q) continue;

        switch (q->op) {
            case assign:
                generate_ASSIGN(q);
                break;
            case add:
                generate_ADD(q);
                break;
            case sub:
                generate_SUB(q);
                break;
            case mul:
                generate_MUL(q);
                break;
            case divv:
                generate_DIV(q);
                break;
            case mod:
                generate_MOD(q);
                break;
            case uminus:
                generate_UMINUS(q);
                break;
            case and_op:
                generate_AND(q);
                break;
            case or_op:
                generate_OR(q);
                break;
            case not_op:
                generate_NOT(q);
                break;
            case if_eq:
                generate_IF_EQ(q);
                break;
            case if_noteq:
                generate_IF_NOTEQ(q);
                break;
            case if_lesseq:
                generate_IF_LESSEQ(q);
                break;
            case if_greatereq:
                generate_IF_GREATEREQ(q);
                break;
            case if_less:
                generate_IF_LESS(q);
                break;
            case if_greater:
                generate_IF_GREATER(q);
                break;
            case call:
                generate_CALL(q);
                break;
            case param:
                generate_PARAM(q);
                break;
            case ret:
                generate_RETURN(q);
                break;
            case getretval:
                generate_GETRETVAL(q);
                break;
            case funcstart:
                generate_FUNCSTART(q);
                break;
            case funcend:
                generate_FUNCEND(q);
                break;
            case tablecreate:
                generate_NEWTABLE(q);
                break;
            case jump:
                generate_JUMP(q);
                break;
            case tablegetelem:
                generate_TABLEGETELEM(q);
                break;
            case tablesetelem:
                generate_TABLESETELEM(q);
                break;
            default:
                std::cerr << "Unknown quad opcode! Aborting.\n";
                assert(0);
        }
    }
}


void free_instructions() {
    for (auto inst : instruction_table) {
        if (inst) free(inst);
    }
    instruction_table.clear();
    currInst = 0;
}

// prints

void print_instructions() {
    for (unsigned int i = 0; i < instruction_table.size(); ++i) {
        instruction *inst = instruction_table[i];
        if (!inst) {
            std::cout << "Instruction " << i << ": null pointer" << std::endl;
            continue;
        }

        std::cout << "Instruction " << i << ": "
                  << "Opcode: " << vmopcode_to_string(inst->opcode);

        if (inst->arg1)
            std::cout << ", Arg1: " << inst->arg1->val;
        else
            std::cout << ", Arg1: null";

        if (inst->arg2)
            std::cout << ", Arg2: " << inst->arg2->val;
        else
            std::cout << ", Arg2: null";

        if (inst->result)
            std::cout << ", Result: " << inst->result->val;
        else
            std::cout << ", Result: null";

        std::cout << ", SrcLine: " << inst->srcLine << std::endl;
    }
}

void print_const_strings(void) {
    for (unsigned i = 0; i < string_vec_consts.size(); i++) {
        std::cout << i << " : " << string_vec_consts[i] << std::endl;
    }
}
void print_const_doubles(void) {
    for (unsigned i = 0; i < double_vec_consts.size(); i++) {
        std::cout << i << " : " << double_vec_consts[i] << std::endl;
    }
}
void print_const_ints(void) {
    for (unsigned i = 0; i < int_vec_consts.size(); i++) {
        std::cout << i << " : " << int_vec_consts[i] << std::endl;
    }
}

void print_libfuncs(void) {
    for (unsigned i = 0; i < lig_strvec_consts.size(); i++) {
        std::cout << i << " : " << lig_strvec_consts[i] << std::endl;
    }
}
void print_userfuncs(void) {
    for (unsigned i = 0; i < funcstack.size(); i++) {
        std::cout << i << " : " << funcstack[i].id << ", "
                  << "Address: " << funcstack[i].address
                  << ", Local Size: " << funcstack[i].localSize << std::endl;
    }
}
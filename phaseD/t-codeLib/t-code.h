#ifndef T_CODE_HEADER
#define T_CODE_HEADER

#include <cassert>
#include <iostream>

#include "../Quads/quad.h"
#include "../Symtable/TableEntry/SymbolTableEntry.h"

typedef void (*generator_func_t)(quad*);

enum vmopcode {
    assign_v,
    add_v,
    sub_v,
    mul_v,
    div_v,
    mod_v,
    uminus_v,
    and_v,
    or_v,
    not_v,
    jeq_v,
    jne_v,
    jle_v,
    jge_v,
    jlt_v,
    jgt_v,
    call_v,
    pusharg_v,
    funcenter_v,
    funcexit_v,
    newtable_v,
    tablegetelem_v,
    tablesetelem_v,
    nop_v,
    jump_v,
    ret_v
};

typedef enum vmarg_t {
    global_a,
    local_a,
    formal_a,
    bool_a,
    string_a,
    number_a,
    userfunc_a,
    libfunc_a,
    retval_a,
    label_a,
    nil_a,
    undef_a,
} vmarg_t;

typedef struct vmarg {
    vmarg_t type;
    unsigned val;
} vmarg;

typedef struct instruction {
    vmopcode opcode;
    vmarg* result;
    vmarg* arg1;
    vmarg* arg2;
    unsigned srcLine;
} instruction;

typedef struct incomplete_jump {
    unsigned instrNo;
    unsigned iaddress;
    incomplete_jump* next;
} incomplete_jump;

incomplete_jump* ij_head = (incomplete_jump*)0;
unsigned ij_total = 0;

void vm_emit(instruction*);
void add_incomplete_jump(unsigned instrNo, unsigned iaddress);
void make_operand(expr* e, vmarg* arg);

unsigned nextinstructionlabel();

unsigned consts_newstring(std::string s);
unsigned consts_newnumber(double n);
unsigned consts_newint(int a);
unsigned libfuncs_newused(std::string s);
unsigned userfunc_newfunc(SymbolTableEntry_T sym);


void make_numberoperand(vmarg* arg, double val);
void make_booloperand(vmarg* arg, unsigned val);
void make_retvaloperand(vmarg* arg);
void reset_operand(vmarg* arg);

void generate(vmopcode op, quad* quad);
void generate_relational(vmopcode op, quad* quad);

void generate_ADD(quad*);
void generate_SUB(quad*);
void generate_DIV(quad*);
void generate_MOD(quad*);
void generate_NEWTABLE(quad*);
void generate_TABLEGETELEM(quad*);
void generate_ASSIGN(quad*);
void generate_NOP(quad*);
void generate_JUMP(quad*);
void generate_IF_EQ(quad*);
void generate_IF_NOTEQ(quad*);
void generate_IF_GREATER(quad*);
void generate_IF_GREATEREQ(quad*);
void generate_IF_LESS(quad*);
void generate_IF_LESSEQ(quad*);
void generate_NOT(quad*);
void generate_PARAM(quad*);
void generate_CALL(quad*);
void generate_GETRETVAL(quad*);
void generate_FUNCSTART(quad*);
void generate_RETURN(quad*);
void generate_FUNCEND(quad*);

#endif
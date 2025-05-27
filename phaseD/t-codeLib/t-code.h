#include <iostream> 
#include <cassert>
#include "../Quads/quad.h" 


enum vmopcode {
    assign_v,
    add_v,
    sub_v,
    mul_v,
    divv_v,
    mod_v,
    uminus_v,
    and_op_v,
    or_op_v,
    not_op_v,
    if_eq_v,
    if_noteq_v,
    if_lesseq_v,
    if_greatereq_v,
    if_less_v,
    if_greater_v,
    call_v,
    param_v,
    ret_v,
    getretval_v,
    funcstart_v,
    funcend_v,
    tablecreate_v,
    jump_v,
    tablegetelem_v,
    tablesetelem_v
};

enum vmarg_t {
    global_a,
    local_a,
    formal_a,
    bool_a,
    string_a,
    number_a,
    userfunc_a,
    libfunc_a,
    retval_a,
    nil_a,
};

typedef struct vmarg{
    vmarg_t     type;
    unsigned    val;
}vmarg;

typedef struct instruction{
    vmopcode    opcode;
    vmarg *     result;
    vmarg *     arg1;
    vmarg *     arg2;
    unsigned    srcLine; 
}instruction;

typedef struct incomplete_jump { 
    unsigned            instrNo;
    unsigned            iaddress;
    incomplete_jump*    next;
}incomplete_jump;

incomplete_jump * ij_head  = (incomplete_jump *)0;
unsigned          ij_total = 0;

void vm_emit(vmopcode op , vmarg * arg1 ,vmarg * arg2,vmarg * result , unsigned srcLine);
void add_incomplete_jump(unsigned instrNo,unsigned iaddress);
void make_operand(expr * e, vmarg * arg);


unsigned consts_newstring   (std::string s);
unsigned consts_newnumber   (double n);
unsigned libfuncs_newused   (std::string s);
unsigned userfunc_newfunc   (SymbolTableEntry_T sym);


void make_numberoperand (vmarg * arg , double val);
void make_booloperand   (vmarg * arg , unsigned val);
void make_retvaloperand (vmarg * arg);

void generate(void);

extern void generate_ADD(quad*);
extern void generate_SUB(quad*);
extern void generate_DIV(quad*);
extern void generate_MOD(quad*);
extern void generate_NENTABLE(quad*);
extern void generate_TABLEGETELEM(quad*);
extern void generate_ASSIGN(quad*);
extern void generate_NOP(quad*);
extern void generate_JUMP(quad*);
extern void generate_IF_EQ(quad*);
extern void generate_IF_NOTEQ(quad*);
extern void generate_IF_GREATER(quad*);
extern void generate_IF_GREATEREQ(quad*);
extern void generate_IF_LESS(quad*);
extern void generate_IF_LESSEQ(quad*);
extern void generate_NOT(quad*);
extern void generate_PARAM(quad*);
extern void generate_CALL(quad*);
extern void generate_GETRETVAL(quad*);
extern void generate_FUNCSTART(quad*);
extern void generate_RETURN(quad*);
extern void generate_FUNCEND(quad*);

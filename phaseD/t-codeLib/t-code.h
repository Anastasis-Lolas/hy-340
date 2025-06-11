#ifndef T_CODE_HEADER
#define T_CODE_HEADER

#include <cassert>
#include <fstream>
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
    label_a,     // 0
    global_a,    // 1
    formal_a,    // 2
    local_a,     // 3
    number_a,    // 4
    string_a,    // 5
    bool_a,      // 6
    nil_a,       // 7
    userfunc_a,  // 8
    libfunc_a,   // 9
    retval_a,    // 10
    undef_a,     // 11
} vmarg_t;

typedef struct vmarg {
    vmarg_t type;
    unsigned val;
} vmarg;

typedef struct instruction {
    vmopcode opcode;
    vmarg result;
    vmarg arg1;
    vmarg arg2;
    unsigned srcLine;
} instruction;

struct userfunc {
    unsigned address;
    unsigned localSize;
    std::string id;
};


typedef struct incomplete_jump {
    unsigned instrNo;
    unsigned iaddress;
    incomplete_jump* next;
} incomplete_jump;

// incomplete_jump* ij_head = (incomplete_jump*)0;
// unsigned ij_total = 0;

void vm_emit(instruction*);
void add_incomplete_jump(unsigned instrNo, unsigned iaddress);
void patch_incomplete_jumps();
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
void generate_MUL(quad* q);
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
void generate_FUNCSTART(quad*);
void generate_UMINUS(quad*);
void generate_TABLESETELEM(quad*);

void generate_instructions();
void generate_AND(quad*);
void generate_OR(quad*);
void generate_RETURN(quad*);
std::string vmopcode_to_string(vmopcode op);
void print_instructions(void);
void print_const_strings(void);
void print_const_nums(void);
void print_userfuncs(void);
void print_libfuncs(void);
void generate_binary_readable(const std::string& outname);
void generate_txt_read(const std::string& outname);
#endif

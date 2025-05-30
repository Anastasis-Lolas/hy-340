#ifndef AVM_EXECUTE_H
#define AVM_EXECUTE_H


#include <iostream>
#include "avm_execute.h"
#include "t-codeLib/t-code.h"
#include "execs/execute_jumps.h"
#include "execs/execute_arith.h"
#include "execs/execute_funcs.h"
#include "execs/execute_tables.h"
#include "memcell_struct.h"


#define execute_add execute_arithmetic 
#define execute_sub execute_arithmetic 
#define execute_mul execute_arithmetic 
#define execute_div execute_arithmetic 
#define execute_mod execute_arithmetic 

//----------J U M P S -----------------//
#define execute_jge execute_rljump
#define execute_jgt execute_rljump 
#define execute_jle execute_rljump 
#define execute_jlt execute_rljump 

typedef unsigned char (*equality_check)(avm_memcell* op1, avm_memcell* op2);
typedef double (*jump_cmp_func) (double x , double y);
typedef double (*arithmetic_func_t)(double x ,double y);

void execute_rljmp(instruction * instr);

double jge_impl (double x , double y);
double jgt_impl (double x , double y);
double jle_impl (double x , double y);
double jlt_impl (double x , double y);


unsigned char check_eq_number(avm_memcell* op1, avm_memcell* op2);
unsigned char check_eq_strings(avm_memcell* op1, avm_memcell* op2);


void execute_jump(instruction*);
void execute_jeq(instruction*);
void execute_jne(instruction*);
void execute_jle(instruction*);
void execute_jge(instruction*);
void execute_jlt(instruction*);
void execute_jgt(instruction*);

//----------J U M P S -----------------//

//---------F U N C S ---------------//
void execute_call(instruction*);
void execute_pusharg(instruction*);
void execute_funcenter(instruction*);
void execute_funcexit(instruction*);
//---------F U N C S ---------------//

//--------------------------------//
double add_impl(double x ,double y);
double sub_impl(double x ,double y);
double mul_impl(double x ,double y);
double div_impl(double x ,double y);
double mod_impl(double x ,double y);
//--------------------------------//

//--------------------------------//
void execute_assign(instruction*);
void avm_assign (avm_memcell * lv , avm_memcell * rv);
void execute_arithmetic(instruction *);
//--------------------------------//

//--------------------------------//
void execute_add(instruction*);
void execute_sub(instruction*);
void execute_mul(instruction*);
void execute_div(instruction*);
void execute_mod(instruction*);
//--------------------------------//

//------------XXXX--------------------//
void execute_uminus(instruction*);
void execute_and(instruction*);
void execute_or(instruction*);
void execute_not(instruction*);
//------------XXXX--------------------//


//----------T A B L E S ------------------//
void execute_newtable(instruction*);
void execute_tablegetelem(instruction*);
void execute_tablesetelem(instruction*);
//----------T A B L E S ------------------//

void avm_execute_cycle(void);
void avm_callsaveenvironment(void);


void execute_nop(instruction*);


void execute_cycle(void);

void execute_funcexit(instruction* unused);


#endif  // AVM_EXECUTE_H
#ifndef EXECUTE_JUMPS_H
#define EXECUTE_JUMPS_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"
#include "../avm_helper.h"


#define execute_jge execute_rljump
#define execute_jgt execute_rljump 
#define execute_jle execute_rljump 
#define execute_jlt execute_rljump 
 

typedef unsigned char (*equality_check)(avm_memcell* op1, avm_memcell* op2);
typedef double (*jump_cmp_func) (double x , double y);

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


#endif
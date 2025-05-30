#ifndef EXECUTE_ARITH_H
#define EXECUTE_ARITH_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"
#include "../avm_table.h"

#define execute_add execute_arithmetic 
#define execute_sub execute_arithmetic 
#define execute_mul execute_arithmetic 
#define execute_div execute_arithmetic 
#define execute_mod execute_arithmetic 

typedef double (*arithmetic_func_t)(double x ,double y);

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


#endif
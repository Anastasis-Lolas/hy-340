#ifndef EXECUTE_ARITH_H
#define EXECUTE_ARITH_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"

//--------------------------------//
void execute_assign(instruction*);
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
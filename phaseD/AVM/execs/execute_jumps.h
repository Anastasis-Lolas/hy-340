#ifndef EXECUTE_JUMPS_H
#define EXECUTE_JUMPS_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"

void execute_jump(instruction*);
void execute_jeq(instruction*);
void execute_jne(instruction*);
void execute_jle(instruction*);
void execute_jge(instruction*);
void execute_jlt(instruction*);
void execute_jgt(instruction*);


#endif
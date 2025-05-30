#ifndef AVM_EXECUTE_H
#define AVM_EXECUTE_H


#include <iostream>

#include "memcell_struct.h"

void avm_execute_cycle(void);
void avm_callsaveenvironment(void);


void execute_assign(instruction*);
void execute_add(instruction*);
void execute_sub(instruction*);
void execute_mul(instruction*);
void execute_div(instruction*);
void execute_mod(instruction*);
void execute_uminus(instruction*);
void execute_and(instruction*);
void execute_or(instruction*);
void execute_not(instruction*);
void execute_jeq(instruction*);
void execute_jne(instruction*);
void execute_jle(instruction*);
void execute_jge(instruction*);
void execute_jlt(instruction*);
void execute_jgt(instruction*);
void execute_call(instruction*);
void execute_pusharg(instruction*);
void execute_funcenter(instruction*);
void execute_funcexit(instruction*);
void execute_newtable(instruction*);
void execute_tablegetelem(instruction*);
void execute_tablesetelem(instruction*);
void execute_jump(instruction*);
void execute_nop(instruction*);


void execute_cycle(void);

void execute_funcexit(instruction* unused);


#endif  // AVM_EXECUTE_H
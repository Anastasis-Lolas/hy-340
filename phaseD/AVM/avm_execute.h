#ifndef AVM_EXECUTE_H
#define AVM_EXECUTE_H


#include <iostream>

#include "memcell_struct.h"

void avm_execute_cycle(void);
void avm_callsaveenvironment(void);


void execute_nop(instruction*);


void execute_cycle(void);

void execute_funcexit(instruction* unused);


#endif  // AVM_EXECUTE_H
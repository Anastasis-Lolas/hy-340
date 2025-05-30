#ifndef EXECUTE_FUNCS_H
#define EXECUTE_FUNCS_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"


void execute_call(instruction*);
void execute_pusharg(instruction*);
void execute_funcenter(instruction*);
void execute_funcexit(instruction*);

#endif
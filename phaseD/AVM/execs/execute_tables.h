#ifndef EXECUTE_TABLES_H
#define EXECUTE_TABLES_H

#include <iostream>
#include "../memcell_struct.h"
#include "../avm_execute.h"


void execute_newtable(instruction*);
void execute_tablegetelem(instruction*);
void execute_tablesetelem(instruction*);


#endif
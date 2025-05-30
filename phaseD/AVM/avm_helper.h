
#ifndef AVM_HELPER_H
#define AVM_HELPER_H
#include <iostream>
#include <string>

#include "avm_table.h"
#include "memcell_struct.h"
#include "t-codeLib/t-code.h"


typedef bool (*tobool_func_t)(avm_memcell*);

bool number_tobool(avm_memcell* m);
bool string_tobool(avm_memcell* m);
bool bool_tobool(avm_memcell* m);
bool table_tobool(avm_memcell* m);
bool userfunc_tobool(avm_memcell* m);
bool libfunc_tobool(avm_memcell* m);
bool nil_tobool(avm_memcell* m);
bool undef_tobool(avm_memcell* m);


#endif  // AVM_HELPER_H
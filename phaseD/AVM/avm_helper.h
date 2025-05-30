
#ifndef AVM_HELPER_H
#define AVM_HELPER_H
#include <iostream>
#include <string>

#include "memcell_struct.h"
#include "t-codeLib/t-code.h"


typedef bool (*tobool_func_t)(avm_memcell*);

bool avm_tobool(avm_memcell* m);
bool number_tobool(avm_memcell* m);
bool string_tobool(avm_memcell* m);
bool bool_tobool(avm_memcell* m);
bool table_tobool(avm_memcell* m);
bool userfunc_tobool(avm_memcell* m);
bool libfunc_tobool(avm_memcell* m);
bool nil_tobool(avm_memcell* m);
bool undef_tobool(avm_memcell* m);

void avm_error(const std::string& msg);
void avm_warning(const std::string& msg);


#endif  // AVM_HELPER_H
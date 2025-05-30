
#ifndef AVM_HELPER_H
#define AVM_HELPER_H
#include <iostream>
#include <string>

#include "memcell_struct.h"
#include "t-codeLib/t-code.h"


typedef bool (*tobool_func_t)(avm_memcell*);
typedef std::string (*tostring_func_t) (avm_memcell*);


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

std::string avm_toString (avm_memcell* m);

std::string number_toString (avm_memcell* m);
std::string string_toString (avm_memcell* m);
std::string bool_toString (avm_memcell* m);
std::string table_toString (avm_memcell* m);
std::string userfunc_toString (avm_memcell* m);
std::string libfunc_toString (avm_memcell* m);
std::string nil_toString (avm_memcell* m);
std::string undef_toString (avm_memcell* m);



#endif  // AVM_HELPER_H
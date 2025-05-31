
#ifndef AVM_HELPER_H
#define AVM_HELPER_H
#include <iostream>
#include <string>

#include "memcell_struct.h"
#include "../t-codeLib/t-code.h"




bool avm_tobool(avm_memcell* );
bool number_tobool(avm_memcell* );
bool string_tobool(avm_memcell* );
bool bool_tobool(avm_memcell* );
bool table_tobool(avm_memcell* );
bool userfunc_tobool(avm_memcell* );
bool libfunc_tobool(avm_memcell* );
bool nil_tobool(avm_memcell* );
bool undef_tobool(avm_memcell* );


std::string number_toString (avm_memcell* m);
std::string string_toString (avm_memcell* m);
std::string bool_toString (avm_memcell* m);
std::string table_toString (avm_memcell* m);
std::string userfunc_toString (avm_memcell* m);
std::string libfunc_toString (avm_memcell* m);
std::string nil_toString (avm_memcell* m);
std::string undef_toString (avm_memcell* m);



void avm_error(const std::string& msg);
void avm_warning(const std::string& msg);

std::string avm_toString (avm_memcell* m);



#endif  // AVM_HELPER_H
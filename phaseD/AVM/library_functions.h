#ifndef LIBRARY_FUNCTIONS_H
#define LIBRARY_FUNCTIONS_H

// #include "../Symtable/symtable.h"
#include "memcell_struct.h"

typedef void (*library_func_t)(void);

library_func_t avm_getlibraryfunc(std::string id);

void libfunc_print();

void libfunc_input();

void libfunc_objectmemberkeys();

void libfunc_objecttotalmembers();

void libfunc_objectcopy();

void libfunc_totalarguments();

void libfunc_argument();

void libfunc_typeof();

void libfunc_strtonum();

void libfunc_sqrt();

void libfunc_sin();

void libfunc_cos();

#endif  // LIBRARY_FUNCTIONS_H

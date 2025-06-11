#ifndef MEMCELL_STRUCT_H
#define MEMCELL_STRUCT_H

#include <iostream>

#include "../t-codeLib/t-code.h"


#define AVM_STACKSIZE 4096
#define AVM_STACKENV_SIZE 4
#define AVM_NUMACTUALS_OFFSET +4
#define AVM_SAVEDPC_OFFSET +3
#define AVM_SAVEDTOP_OFFSET +2
#define AVM_SAVEDTOPSP_OFFSET +1
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))


struct avm_table;  // Forward declaration for table type

enum avm_memcell_t {
    number_m = 0,
    string_m = 1,
    bool_m = 2,
    table_m = 3,
    userfunc_m = 4,
    libfunc_m = 5,
    nil_m = 6,
    undef_m = 7
};


typedef struct avm_memcell {
    avm_memcell_t type;
    double numVal;
    std::string strVal;
    bool boolVal;
    struct avm_table* tableVal;
    unsigned funcVal;
    std::string libfuncVal;
} avm_memcell;


typedef void (*memclear_func_t)(avm_memcell*);

extern memclear_func_t memclearFuncs[];


avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg);

void avm_dec_top(void);
void avm_push_envvalue(unsigned val);
void avm_callsaveenvironment(void);

void avm_initstack(void);

double consts_getnumber(unsigned index);
std::string consts_getstring(unsigned index);
std::string libfunc_get(unsigned index);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);
void avm_memcellclear(avm_memcell* m);

#endif

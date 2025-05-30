#ifndef MEMCELL_STRUCT_H
#define MEMCELL_STRUCT_H

#include <iostream>

#include "t-codeLib/t-code.h"

#define AVM_STACKSIZE 4096
#define AVM_STACKENV_SIZE 4
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))
typedef void (*memclear_func_t)(avm_memcell*);

extern memclear_func_t memclearFuncs[];

enum avm_memcell_t {
    double_m = 0,
    int_m = 1,
    string_m = 2,
    bool_m = 3,
    table_m = 4,
    userfunc_m = 5,
    libfunc_m = 6,
    nil_m = 7,
    undef_m = 8
};

struct avm_table;  // Forward declaration for table type

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double doubleVal;
        int intVal;
        std::string strVal;
        bool boolVal;
        struct avm_table* tableVal;
        unsigned
            funcVal; /* changed from unsigned (why), changed from usefunc */
        std::string libfuncVal;
    } data;

} avm_memcell;


avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg);

void avm_callsaveenvironment(void);

void avm_dec_top(void);


void avm_initstack(void);

double consts_getdouble(unsigned index);

int consts_getint(unsigned index);
std::string consts_getstring(unsigned index);
std::string libfunc_get(unsigned index);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);


#endif

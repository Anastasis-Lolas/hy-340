#ifndef MEMCELL_STRUCT_H
#define MEMCELL_STRUCT_H

#include <iostream>

#include "t-codeLib/t-code.h"

#define AVM_STACKSIZE 4096
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))


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

struct avm_table;  // Forward declaration for table type

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double numVal;
        std::string* strVal;
        bool boolVal;
        struct avm_table* tableVal;
        userfunc funcVal; /* changed from unsigned */
        std::string* libfuncVal;
    } data;

} avm_memcell;


avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg);

void avm_callsaveenvironment(void);

void avm_dec_top(void);


void avm_initstack(void);

#endif
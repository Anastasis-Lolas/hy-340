#include "avm_execute.h"
#include "memcell_struct.h"
#include "t-codeLib/t-code.h"


avm_memcell stack[AVM_STACKSIZE];

unsigned char executionFinished = 0;
unsigned pc = 0;
// unsigned        currLine = 0;
unsigned codeSize = 0;

void avm_initstack(void) {
    for (size_t i = 0; i < AVM_STACKSIZE; ++i) {
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

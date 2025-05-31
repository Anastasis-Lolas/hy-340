#include <cstring>

#include "avm_execute.h"
#include "memcell_struct.h"


#define AVM_ENDING_PC codeSize

avm_memcell stack[AVM_STACKSIZE];


unsigned pc = 0;  // Program counter
unsigned executionFinished = 0;
unsigned currLine = 0;  // Current line number
unsigned codeSize = 0;
instruction* code = nullptr;  // Pointer to the code array
unsigned totalActuals = 0;

avm_memcell ax, bx, cx;
avm_memcell retval; // ERROR here
int top, topsp;


void avm_initstack(void) {
    for (size_t i = 0; i < AVM_STACKSIZE; ++i) {
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg) {
    assert(arg);

    switch (arg->type) {
        case global_a:
            return &stack[AVM_STACKSIZE - 1 - arg->val];
        case local_a:
            return &stack[topsp - arg->val];
        case formal_a:
            return &stack[topsp + AVM_STACKENV_SIZE + arg->val];
        case retval_a:
            return &retval;
        case number_a:
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        case string_a:
            reg->type = string_m;
            reg->data.strVal = consts_getstring(arg->val);
            return reg;
        case bool_a:
            reg->type = bool_m;
            reg->data.boolVal = (arg->val != 0);
            return reg;
        case nil_a:
            reg->type = nil_m;
            return reg;
        case userfunc_a:
            reg->type = userfunc_m;
            reg->data.funcVal = arg->val;
            // reg->data.funcVal = userfunc_get(arg->val)->address;
            return reg;
        case libfunc_a:
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfunc_get(arg->val);
            return reg;
        default:
            std::cerr << "Invalid operand type: " << arg->type << std::endl;
            assert(0);
    }
}


double consts_getnumber(unsigned index) { return nums_consts[index]; }

std::string consts_getstring(unsigned index) { return string_consts[index]; }

std::string libfunc_get(unsigned index) { return libfuncs[index]; }

void avm_memcellclear(avm_memcell* m) {
    assert(m);
    if (m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if (f) f(m);
        m->type = undef_m;
    }
}

void memclear_string(avm_memcell* m) {
    assert(!m->data.strVal.empty());
    m->data.strVal.clear();
    // delete m->data.strVal;
}

void memclear_table(avm_memcell* m) {
    assert(m->data.tableVal);
    avm_tabledecrefcounter(m->data.tableVal);
}

memclear_func_t memclearFuncs[] = {
    0, /* number */
    memclear_string,
    0, /* bool */
    memclear_table,
    0, /* userfunc */
    0, /* libfunc */
    0, /* nil */
    0  /* undef */
};


void avm_dec_top(void) {
    if (!top) { /* Stack overflow */
        avm_error("Stack overflow");
        executionFinished = 1;
    } else {
        top--;
    }
}

void avm_push_envvalue(unsigned val) {
    stack[top].type = number_m;
    stack[top].data.numVal = val;  // Copy the data
    avm_dec_top();
}

void avm_callsaveenvironment(void) {
    avm_push_envvalue(totalActuals);
    assert(code[pc].opcode == call_v);
    avm_push_envvalue(pc + 1);
    avm_push_envvalue(top + totalActuals + 2);
    avm_push_envvalue(topsp);
}

userfunc* avm_getfuncinfo(unsigned address) {
    if (address < userfuncs.size()) {
        return &userfuncs[address];
    } else {
        avm_error("Invalid function address: " + std::to_string(address));
        // error??
        return nullptr;
    }
}
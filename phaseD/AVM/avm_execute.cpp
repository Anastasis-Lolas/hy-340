#include "avm_execute.h"

#define AVM_ENDING_PC codeSize


extern unsigned pc = 0;  // Program counter
extern unsigned executionFinished = 0;
unsigned currLine = 0;  // Current line number
extern unsigned codeSize = 0;


typedef void (*execute_func_t)(instruction*);

execute_func_t executeFuncs[] = {
    execute_assign,       execute_add,          execute_sub,
    execute_mul,          execute_div,          execute_mod,
    execute_uminus,       execute_and,          execute_or,
    execute_not,          execute_jeq,          execute_jne,
    execute_jle,          execute_jge,          execute_jlt,
    execute_jgt,          execute_call,         execute_pusharg,
    execute_funcenter,    execute_funcexit,     execute_newtable,
    execute_tablegetelem, execute_tablesetelem, execute_jump,
    execute_nop};
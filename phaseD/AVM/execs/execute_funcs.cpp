#include "../avm_execute.h"

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(instr->result, &ax);
    assert(func);
    if (funcAddress >= funcstack.size()) {
        std::cerr << "Error: Function address out of bounds." << std::endl;
        return;
    }
    userfunc& func = funcstack[funcAddress];
    avm_call(func.address, func.localSize);
}
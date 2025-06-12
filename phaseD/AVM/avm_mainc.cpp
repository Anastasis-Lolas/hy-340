#include <iostream>

#include "../t-codeLib/t-code.h"
#include "avm_execute.h"
#include "avm_table.h"
#include "memcell_struct.h"


int main() {
    // First we are going to read the binary file
    read_and_print_avm_binary("../bifile.abc");
    print_all();
    init_lib_functions();
    avm_initstack();
    execute_cycle();
    std::cout << std::endl;
}
#include <iostream>

#include "../t-codeLib/t-code.h"
#include "avm_execute.h"
#include "avm_table.h"
#include "memcell_struct.h"


int main() {
    // First we are going to read the binary file
    read_and_print_avm_binary("../bifile.abc");
    execute_cycle();
}
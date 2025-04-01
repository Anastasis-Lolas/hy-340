#ifndef HANDLER_HEADER
#define HANDLER_HEADER
#include <iostream>
#include <vector>

#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"

unsigned int scope = 0;
unsigned int func_num = 0;
SymTable_T oSymTable = SymTable_new();
int yylineno;
ScopeList_T scopeList;

std::string create_func_name(void) { return "_f" + std::to_string(func_num++); }


void add_function(std::string name, std::vector<void*> args) {
    SymbolTableEntry_T entry = nullptr;
    void* result;
    int offset;
    if (name == "") {
        name = create_func_name();
    } else {
        result = SymTable_lookup(oSymTable, name);
        if (result) {
            entry = static_cast<SymbolTableEntry_T>(result);
        }
    }
    if (entry) {
        if (entry->type == LIBFUNC) {
            // print error message shadows lib function
            std::cout << "Error: function " << name << "  shadows lib function"
                      << std::endl;
        } else {
            if (entry->type == USERFUNC) {
                // print error message function already declared in the same
                // scope
                std::cout << "Error: function " << name << " already declared"
                          << std::endl;
            } else {
                // print error message function-same name as variable
                std::cout << "Error: function " << name
                          << " -->>>>>> same name as variable" << std::endl;
            }
        }
        return;
    }
    offset = find_offset(scopeList, scope);

    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);

    SymTable_put(oSymTable, name, entry);
}
int main() {
    int arg1 = 42;
    std::string arg2 = "Hello";
    std::vector<void*> args = {&arg1, &arg2};

    add_function("Function1", args);
    add_function("Function1", {});
    add_function("Function2", {});
    SymTable_print(oSymTable);

    return 0;
}


#endif
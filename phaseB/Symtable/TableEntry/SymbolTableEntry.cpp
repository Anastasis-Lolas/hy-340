#include <cassert>
#include <iostream>
// #include <string>
#include <vector>

#include "SymbolTableEntry.h"


/* Creates a new scope list */
SymbolTableEntry_T SymTableEntry_new(SymbolType type, std::string name,
                                     unsigned int scope, unsigned int line,
                                     int offset, std::vector<void *> args) {
    SymbolTableEntry_T entry = new SymbolTableEntry;
    if (entry == NULL) {
        printf("error-out of memory\n");
        exit(-1);
    }
    entry->isActive = true;
    entry->type = type;
    entry->value.varVal = new Variable;
    if (entry->value.varVal == NULL) {
        printf("error-out of memory\n");
        exit(-1);
    }
    entry->value.varVal->name = name;
    entry->value.varVal->scope = scope;
    entry->value.varVal->line = line;

    // Or I will find it manually
    entry->value.varVal->offset = offset;

    if (type == USERFUNC || type == LIBFUNC) {
        entry->value.funcVal = new Function;
        if (entry->value.funcVal == NULL) {
            printf("error-out of memory\n");
            exit(-1);
        }
        entry->value.funcVal->args = args;
    }

    return entry;
}
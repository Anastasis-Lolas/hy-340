#include "SymbolTableEntry.h"

#include <cassert>
#include <iostream>
#include <vector>


/*Creates a new entry for the Symbol Table */
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
    if (type == USERFUNC || type == LIBFUNC) {
        entry->value.funcVal = new Function;
        if (entry->value.funcVal == NULL) {
            printf("error-out of memory\n");
            exit(-1);
        }
        entry->value.funcVal->args = args;
        entry->value.funcVal->name = name;
        entry->value.funcVal->scope = scope;
        entry->value.funcVal->line = line;
        // Or I will find it manually
        entry->value.funcVal->offset = offset;
    } else {
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
    }

    //std::cout << "SymboleEntry_new: " << entry->value.varVal->name << std::endl;
    return entry;
}

/*Frees the memory used by the Symbol Table Entry */
void SymTableEntry_free(SymbolTableEntry_T entry) {
    assert(entry);
    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        delete entry->value.funcVal;
    } else {
        delete entry->value.varVal;
    }
    delete entry;
}

#include <iostream>
#include <vector>

typedef struct Variable {
    std::string name;
    unsigned int scope;
    unsigned int line;
    unsigned int offset;
} Variable;


typedef struct Function {
    std::string name;
    std::vector<void *> args;
    unsigned int scope;
    unsigned int line;
    unsigned int offset;
} Function;

enum SymbolType { GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC } SymbolType_t;

typedef struct SymbolTableEntry {
    bool isActive;
    union {
        Variable *varVal;
        Function *funcVal;
    } value;
    enum SymbolType type;
} *SymbolTableEntry_T;

SymbolTableEntry_T SymTableEntry_new(SymbolType type, std::string name,
                                     unsigned int scope, unsigned int line,
                                     int offset, std::vector<void *> args);
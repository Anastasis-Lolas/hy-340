#include <iostream>
#include <vector>

typedef struct Variable {
    const std::string name;
    unsigned int scope;
    unsigned int line;
} Variable;


typedef struct Function {
    const std::string name;
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
} SymbolTableEntry;
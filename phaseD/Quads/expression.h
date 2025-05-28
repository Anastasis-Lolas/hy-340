#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "../Symtable/TableEntry/SymbolTableEntry.h"

enum expr_t {
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constdouble_e,
    intnum_e,
    constbool_e,
    conststring_e,

    nil_e
};

typedef struct expr {
    expr_t type;
    SymbolTableEntry_T sym;
    expr* index;
    double numConst;
    int numInt;
    std::string strConst;
    bool boolConst;
    std::vector<int> truelist;
    std::vector<int> falselist;
    expr* next;

} expr;

typedef struct call_t {
    expr* elist;
    unsigned char method;
    std::string name;
} call_t;


#endif
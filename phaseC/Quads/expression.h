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

    constnum_e,
    constbool_e,
    conststring_e,

    nil_e
};

typedef struct expr {
    expr_t type;
    SymbolTableEntry_T sym;
    expr* index;
    double numConst;
    std::string strConst;
    bool boolConst;
    std::vector<unsigned> truelist;  
    std::vector<unsigned> falselist;
    expr* next;

} expr;


#endif
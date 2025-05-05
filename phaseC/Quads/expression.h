#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../Symtable/TableEntry/SymbolTableEntry.h"
#include <string>

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
    expr_t              type;
    SymbolTableEntry    sym;
    expr*               index;
    double              numConst;
    std::string         strConst;
    bool                boolConst;
    expr *              next;

} expr;

#endif
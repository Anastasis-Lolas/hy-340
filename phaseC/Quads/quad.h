#ifndef QUAD_H
#define QUAD_H

#include "expression.h"



enum iopcode {
    assign,         add,            sub,
    mul,            div,          mod,
    uminus,         and_op,            or_op,   
    not_op,          if_eq,          if_noteq,
    if_lesseq,      if_greatereq,   if_less,   
    if_greater,     call,           param, 
    ret,            getretval,      funcstart,
    funcend,        tablecreate,    
    tablegetelem,   tablesetelem
};

typedef struct quad {
    iopcode     op;
    expr*       result;
    expr*       arg1;
    expr*       arg2;
    unsigned    label;
    unsigned    line;    
} quad;




#endif

#include "quad.h"
#include <iostream>
#include <vector>

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line) {
    quad newQuad;

    newQuad.op = op;
    newQuad.arg1 = arg1;
    newQuad.arg2 = arg2;
    newQuad.result = result;
    newQuad.label = label;
    newQuad.line = line;

    quad_table.push_back(newQuad);
}

#include "quad.h"
#include <iostream>
#include <vector>
#include <cassert>

std::vector<quad *> quad_table;

unsigned int total = 0;

quad * quads = (quad * ) 0;
unsigned  currQuad = 0; 

unsigned nextquadlabel(void){
    return currQuad;
}

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line) {
    quad * newQuad;

    newQuad = quads + currQuad++; 

    newQuad->op = op;
    newQuad->arg1 = arg1;
    newQuad->arg2 = arg2;
    newQuad->result = result;
    newQuad->label = label;
    newQuad->line = line;

    quad_table.push_back(newQuad);
}

void patchlabel(unsigned quadNo, unsigned label) {
    // Check if quadNo is within the bounds of the vector and if label is not already set
    assert(quadNo < currQuad && !quads[quadNo].label);

    // Update the label at the given index
    quads[quadNo].label = label;
}
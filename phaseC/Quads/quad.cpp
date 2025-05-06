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

void patchlabel(unsigned quadNo, unsigned label) {
    // Check if quadNo is within the bounds of the vector and if label is not already set
    if(quadNo < currQuad)
        std::cout<<"quadNo less than currQuad"<<std::endl;

    // Update the label at the given index
    quad_table[quadNo]->label = label;
}

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line) {
   
    quad* quadd = (quad*)malloc(sizeof(quad));

    if (!quadd) {
        std::cerr << "Error: Memory allocation failed for quad." << std::endl;
        return;
    }

    quadd->op 		= op;
	quadd->arg1		= arg1;
	quadd->arg2		= arg2;
	quadd->result	= result;
	quadd->label 	= label;
	quadd->line		= line;
    quad_table.push_back(quadd);
 
}

// void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line) {
//     quad * newQuad;

//     newQuad = quads + currQuad++; 

//     newQuad->op = op;
//     newQuad->arg1 = arg1;
//     newQuad->arg2 = arg2;
//     newQuad->result = result;
//     newQuad->label = label;
//     newQuad->line = line;

//     quad_table.push_back(newQuad);
// }
std::string iopcode_to_string(iopcode op) {
    switch (op) {
        case assign: return "assign";
        case add: return "add";
        case sub: return "sub";
        case mul: return "mul";
        case divv: return "div";
        case mod: return "mod";
        case uminus: return "uminus";
        case and_op: return "and";
        case or_op: return "or";
        case not_op: return "not";
        case if_eq: return "if_eq";
        case if_noteq: return "if_noteq";
        case if_lesseq: return "if_lesseq";
        case if_greatereq: return "if_greatereq";
        case if_less: return "if_less";
        case if_greater: return "if_greater";
        case call: return "call";
        case param: return "param";
        case ret: return "ret";
        case getretval: return "getretval";
        case funcstart: return "funcstart";
        case funcend: return "funcend";
        case tablecreate: return "tablecreate";
        case tablegetelem: return "tablegetelem";
        case tablesetelem: return "tablesetelem";
        default: return "unknown";
    }
}

std::string expr_to_string(expr* e) {
  return "oxi";
}


void print_quads() {
    std::cout << "----- QUAD TABLE -----\n";
    std::cout << "Total quads: " << quad_table.size() << "\n\n";

    for (size_t i = 0; i < quad_table.size(); ++i) {
        quad* q = quad_table[i];
        if (!q) {
            std::cout << i << ": <null quad>\n";
            continue;
        }

        // print index and opcode
        std::cout << i << ": " 
                  << iopcode_to_string(q->op) 
                  << "   (";

        // arg1
        std::cout << expr_to_string(q->arg1) << ", ";

        // arg2
        std::cout << expr_to_string(q->arg2) << ", ";

        // result
        std::cout << expr_to_string(q->result) << ")";

        // label and line
        std::cout << "   [label=" << q->label
                  << ", line="  << q->line << "]\n";
    }

    std::cout << "----------------------\n";
}

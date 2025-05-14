#include "quad.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <iostream>
#include <iomanip> 
std::vector<quad*> quad_table;

unsigned int total = 0;

quad* quads = (quad*)0;
struct lc_stack_t *lcs_top = 0, *lcs_bottom = 0;
unsigned int currQuad = 0;



unsigned nextquadlabel(void) { return currQuad; }

void patchlabel(int quadNo, unsigned label) {
    if (quadNo < 1 || quadNo > quad_table.size()) {
        std::cerr << "Error: Invalid quad number " << quadNo << std::endl;
        return;
    }
    if (quad_table[quadNo - 1] == nullptr) {
        std::cerr << "Error: Quad " << quadNo << " is null." << std::endl;
        return;
    }
    quad_table[quadNo]->label = label;
}

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label,
          unsigned line) {
    quad* quadd = (quad*)malloc(sizeof(quad));

    if (!quadd) {
        std::cerr << "Error: Memory allocation failed for quad." << std::endl;
        return;
    }

    quadd->op = op;
    quadd->arg1 = arg1;
    quadd->arg2 = arg2;
    quadd->result = result;
    quadd->label = label;
    quadd->line = line;
    quad_table.push_back(quadd);

    currQuad++;
}

// void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label,
// unsigned line) {
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
        case assign:
            return "assign";
        case add:
            return "add";
        case sub:
            return "sub";
        case mul:
            return "mul";
        case divv:
            return "div";
        case mod:
            return "mod";
        case uminus:
            return "uminus";
        case and_op:
            return "and";
        case or_op:
            return "or";
        case not_op:
            return "not";
        case if_eq:
            return "if_eq";
        case if_noteq:
            return "if_noteq";
        case if_lesseq:
            return "if_lesseq";
        case if_greatereq:
            return "if_greatereq";
        case if_less:
            return "if_less";
        case if_greater:
            return "if_greater";
        case call:
            return "call";
        case param:
            return "param";
        case ret:
            return "ret";
        case getretval:
            return "getretval";
        case funcstart:
            return "funcstart";
        case funcend:
            return "funcend";
        case tablecreate:
            return "tablecreate";
        case tablegetelem:
            return "tablegetelem";
        case tablesetelem:
            return "tablesetelem";
        case jump:
            return "jump";
        default:
            return "unknown";
    }
}

std::string expr_to_string(expr* e) {
    if (!e) return "";

    // get name 
    if (e->sym) {
        switch (e->sym->type) {
            case GLOBAL:
            case LLOCAL:
            case FORMAL:
                if (e->sym->value.varVal && !e->sym->value.varVal->name.empty()) {
                    return e->sym->value.varVal->name;
                }
                break;
            case USERFUNC:
            case LIBFUNC:
                if (e->sym->value.funcVal && !e->sym->value.funcVal->name.empty()) {
                    return e->sym->value.funcVal->name;
                }
                break;
        }
    }

    // if sym failed
    switch (e->type) {
        case constnum_e:
            if (e->numConst == static_cast<int>(e->numConst)) {
                return std::to_string(static_cast<int>(e->numConst));
            }
            return std::to_string(e->numConst);
        case conststring_e:
            return "\"" + e->strConst + "\"";
        case constbool_e:
            return e->boolConst ? "\"true\"" : "\"false\"";
        case nil_e:
            return "nil";
        default:
            return "_";  // fallback for unnamed temp
    }
}


// void print_quads() {
//     std::cout << "----- QUAD TABLE -----\n";
//     std::cout << "Total quads: " << quad_table.size() << "\n\n";

//     for (size_t i = 0; i < quad_table.size(); ++i) {
//         quad* q = quad_table[i];
//         if (!q) {
//             std::cout << i << ": <null quad>\n";
//             continue;
//         }

//         // print index and opcode
        
//         std::cout << i<< ": " << iopcode_to_string(q->op) << "   (";

//         // arg1
//         std::cout << expr_to_string(q->arg1) << ", ";

//         // arg2
//         std::cout << expr_to_string(q->arg2) << ", ";

//         // result
//         std::cout << expr_to_string(q->result) << ")";

//         // label and line
//         std::cout << "   [label=" << q->label << ", line=" << q->line << "]\n";
//     }

//     std::cout << "----------------------\n";
// }


std::string safe_expr_to_string(expr* e) {
    if (!e) return "";
    std::string s = expr_to_string(e);
    if (s == "0" || s == "unknown") return "";
    return s;
}


void print_quads() {
    const int W_IDX    = 8;
    const int W_OP     = 16;
    const int W_RES    = 14;
    const int W_ARG1   = 14;
    const int W_ARG2   = 14;
    const int W_LABEL  = 8;

   std::cout << "------------------------------- QUAD TABLE -------------------------------\n";
    std::cout << std::left
              << std::setw(W_IDX)   << "quad #"
              << std::setw(W_OP)    << "opcode"
              << std::setw(W_RES)   << "result"
              << std::setw(W_ARG1)  << "arg1"
              << std::setw(W_ARG2)  << "arg2"
              << std::setw(W_LABEL) << "label"
              << "\n";

    std::cout << std::string(W_IDX + W_OP + W_RES + W_ARG1 + W_ARG2 +W_LABEL, '-')
              << "\n";

    for (size_t i = 0; i < quad_table.size(); ++i) {
        quad* q = quad_table[i];
        if (!q) continue;  // skip null quads

        std::string result = safe_expr_to_string(q->result);
        std::string arg1   = safe_expr_to_string(q->arg1);
        std::string arg2   = safe_expr_to_string(q->arg2);
        std::string label  = q->label == 0 ? "" : std::to_string(q->label);

        std::cout << std::left
                  << std::setw(W_IDX)   << i
                  << std::setw(W_OP)    << iopcode_to_string(q->op)
                  << std::setw(W_RES)   << result
                  << std::setw(W_ARG1)  << arg1
                  << std::setw(W_ARG2)  << arg2
                  << std::right
                  << std::setw(W_LABEL) << label
                  << std::left << "\n";
    }

    std::cout << std::string(W_IDX + W_OP + W_RES + W_ARG1 + W_ARG2 + W_LABEL, '-')
              << "\n";
}

void patchlist(int list, int label) {
    while (list) {
        int next = quad_table[list]->label;
        quad_table[list]->label = label;
        list = next;
    }
}

int mergelist(int l1, int l2) {
    if (!l1)
        return l2;
    else if (!l2)
        return l1;
    else {
        int i = l1;

        while (quad_table[i]->label) i = quad_table[i]->label;

        quad_table[i]->label = l2;

        return l1;
    }
}

void make_stmt(stmt_t* s) {
    s->breakList = s->contList = 0;
    s->returnList = 0;
}

int newlist(int i) {
    quad_table[i]->label = 0;
    return i;
}

unsigned nextquad(void) { return currQuad; }


void make_loop_t(forloop_t* loop) { loop->enter = loop->test = 0; }

void push_loopcounter(void){

    struct lc_stack_t * new_stack = new lc_stack_t;

    new_stack->counter = 0;
    new_stack->next = lcs_top;
    lcs_top = new_stack;

    if (!lcs_bottom) lcs_bottom = new_stack;

}

void pop_loopcounter(void){

    if (lcs_top) {
        struct lc_stack_t* temp = lcs_top;
        lcs_top = lcs_top->next;
        free(temp);

        if (!lcs_top)
            lcs_bottom = nullptr;
    }      
}


void backpatch(std::vector<unsigned>& lst, unsigned label) {
    // Iterate over each quad number in the list
    for (unsigned quadNo : lst) {
        // Update the label of the quad at quadNo with the provided label
        patchlabel(quadNo, label);
    }
}

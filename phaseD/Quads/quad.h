#ifndef QUAD_H
#define QUAD_H

#include "expression.h"


enum iopcode {
    assign,
    add,
    sub,
    mul,
    divv,
    mod,
    uminus,
    and_op,
    or_op,
    not_op,
    if_eq,
    if_noteq,
    if_lesseq,
    if_greatereq,
    if_less,
    if_greater,
    call,
    param,
    ret,
    getretval,
    funcstart,
    funcend,
    tablecreate,
    jump,
    tablegetelem,
    tablesetelem
};

typedef struct quad {
    iopcode op;
    expr* result;
    expr* arg1;
    expr* arg2;
    unsigned taddress;
    unsigned label;
    unsigned line;
} quad;

struct stmt_t {
    int breakList;
    int contList;
    int returnList;
};

struct forloop_t {
    unsigned test;
    unsigned enter;
};

struct lc_stack_t {
    struct lc_stack_t* next;
    unsigned counter;
};

extern struct lc_stack_t *lcs_top, *lcs_bottom;

#define loopcounter (lcs_top->counter)

unsigned nextquadlabel(void);

void print_quads();

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label,
          unsigned line);

void patchlabel(int quadNo, unsigned label);

void patchlist(int list, int label);

int mergelist(int l1, int l2);

void make_stmt(stmt_t* s);

void make_loop_t(forloop_t* loop);

int newlist(int i);

unsigned nextquad(void);

void push_loopcounter(void);


void pop_loopcounter(void);

void backpatch(std::vector<int> list, unsigned label);

std::vector<int> merge(const std::vector<int>& lst1,
                       const std::vector<int>& lst2);

std::string iopcode_to_string(iopcode op);
std::string expr_to_string(expr* e);
std::string safe_expr_to_string(expr* e);

#endif

#ifndef EXPR_HANDLER_H
#define EXPR_HANDLER_H

#include <iostream>
#include <vector>

#include "Quads/expression.h"
#include "Quads/quad.h"
#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"

extern unsigned int scope;
extern unsigned int func_num;
extern SymTable_T oSymTable;
extern int yylineno;
extern ScopeList_T scopeList;

void init_tables();
std::string create_func_name(void);
void add_function(std::string name, std::vector<void*> args);
SymbolTableEntry_T add_ident(std::string name);
SymbolTableEntry_T add_local_dent(std::string name);
SymbolTableEntry_T handle_namespace_dent(std::string name);
void printFullSymTable(SymTable_T table);
void exit_block();
void temrs_error(expr* entry, std::string op);
void member_error(expr* entry, std::string memrule);
void assign_error(expr* entry);
void null_entry(expr* entry, std::string message);
// debug
void print_entry(SymbolTableEntry_T entry);
std::vector<void*> handle_func_args(std::vector<void*> args, std::string name);
void print_args(std::vector<void*> args);
int find_line(std::string name);
void enter_func(int flag, std::string name);
void normal_call(SymbolTableEntry_T entry);


expr* newexpr(expr_t t);
expr* newexpr_constnum(double i);
expr* newexpr_conststring(std::string str);
expr* newexpr_bool(bool b);


void resettemp();
std::string newtempname();
SymbolTableEntry_T newtemp();

expr* newexpr(expr_t t);
expr* newexpr_conststring(std::string str);
expr* emit_iftableitem(expr* e);

// Member access
expr* lvalue_id_handler(expr* lvalue, std::string name);
expr* member_handler(expr* lvalue, expr* index);
expr* member_item(expr* lvalue, std::string name);

// Expression emitters
expr* emit_assign_expr(expr* lval, expr* rval);
expr* emit_arith_op(iopcode op, expr* e1, expr* e2);
expr* emit_relop_op(iopcode op, expr* e1, expr* e2);


//
expr* symEntr_to_expr(SymbolTableEntry_T entry);

#endif

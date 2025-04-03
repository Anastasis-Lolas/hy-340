#ifndef EXPR_HANDLER_H
#define EXPR_HANDLER_H

#include <iostream>
#include <vector>

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
void temrs_error(SymbolTableEntry_T entry, std::string op);
void member_error(SymbolTableEntry_T entry, std::string memrule);
void assign_error(SymbolTableEntry_T entry);
void null_entry(SymbolTableEntry_T entry, std::string message);
// debug
void print_entry(SymbolTableEntry_T entry);
std::vector<void *> handle_func_args(std::vector<void *> args,std::string name);
void print_args(std::vector<void *> args);

int find_line(std::string name);
#endif

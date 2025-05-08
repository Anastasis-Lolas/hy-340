#ifndef SCOPELIST_HEADER
#define SCOPELIST_HEADER

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include "../TableEntry/SymbolTableEntry.h"
#include "../symtable.h"


// ScopeList_T is a vector of lists cointaining all entries for each scope
using ScopeList_T = std::vector<std::list<SymbolTableEntry_T> >;

enum scopespace_t { programvar, functionlocal, formalarg };

const std::string LIBS_FUNC[] = {"print",
                                 "input",
                                 "objectmemberkeys",
                                 "objecttotalmembers",
                                 "objectcopy",
                                 "totalarguments",
                                 "argument",
                                 "typeof",
                                 "strtonum",
                                 "sqrt",
                                 "cos",
                                 "sin"};
// std::vector<std::list<SymbolTableEntry_T>> scopeList;

void add_entry(ScopeList_T& scopeList, SymbolTableEntry_T entry, int scope);

void deactivate_scope(ScopeList_T& scopeList, int scope);

void reactivate_scope(ScopeList_T& scopeList, int scope);

void delete_scopelist(ScopeList_T& scopeList, int scope);

SymbolTableEntry_T lookup_within_scope(const ScopeList_T& scopeList,
                                       const std::string& id, int scope);

SymbolTableEntry_T lookup_within_scope(const ScopeList_T& scopeList,
                                       const std::string& id, int scope,
                                       bool isActive);

SymbolTableEntry_T lookup_active(const ScopeList_T& scopeList,
                                 const std::string& id, int scope);
SymbolTableEntry_T lookup_in_list(ScopeList_T& scopeList, const std::string& id,
                                  int scope);

ScopeList_T get_active_vector(ScopeList_T& scopeList);

void init_LIBS_FUNC(ScopeList_T& scopeList, SymTable_T oSymTable);

int search_LIBS_FUNC(std::string& name);

int find_offset(const ScopeList_T& scopeList, int scope);

void print_scopeList(ScopeList_T& scopeList);


void scope_nodes_remove(ScopeList_T& scopeList, int scope);


void enterscopespace(void);
void exitscopespace(void);

scopespace_t currscopespace(void);
unsigned currscopeoffset(void);
void resetformalargsoffset(void);
void incurrscopeoffset(void);

void resetformalargsoffset(void);
void resetformalargsoffset(void);
void resetfunctionlocaloffset(void);
void restorecurrscopeoffset(unsigned n);

void print_offset();

#endif

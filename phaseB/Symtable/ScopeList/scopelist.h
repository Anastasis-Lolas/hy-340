#include <iostream>
#include <list>
#include <vector>

#include "../TableEntry/SymbolTableEntry.h"

using ScopeList_T = std::vector<std::list<SymbolTableEntry_T>>;

// std::vector<std::list<SymbolTableEntry_T>> scopeList;

void add_entry(ScopeList_T& scopeList, SymbolTableEntry_T entry);

void deactivate_scope(ScopeList_T& scopeList, int scope);

void delete_scopelist(ScopeList_T& scopeList, int scope);

#include "scopelist.h"

#include <cassert>


void add_entry(ScopeList_T& scopeList, SymbolTableEntry_T entry) {
    assert(entry);

    int scope = entry->value.varVal->scope;
    if (scopeList.size() <= scope) {
        scopeList.resize(scope + 1);
    }
    scopeList[scope].push_back(entry);
    return;
}

void deactivate_scope(ScopeList_T& scopeList, int scope) {
    assert(scope >= 0 && scope < scopeList.size());
    for (auto& entry : scopeList[scope]) {
        entry->isActive = false;
    }
    return;
}

void delete_scopelist(ScopeList_T& scopeList, int scope) {
    assert(scope >= 0 && scope < scopeList.size());
    scopeList[scope].clear();
    scopeList.shrink_to_fit();
    return;
}

ScopeList_T& get_active_vector(ScopeList_T& scopeList) {
    ScopeList_T activeVector;
    activeVector.resize(scopeList.size());

    for (size_t scope = 0; scope < scopeList.size(); scope++) {
        for (auto& entry : scopeList[scope]) {
            if (!entry->isActive) {
                activeVector[scope].push_back(entry);
            }
        }
    }
    return activeVector;
}
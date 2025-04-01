#include "scopelist.h"

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

void init_LIBS_FUNC(ScopeList_T& scopeList,SymTable_T oSymTable){

   
     for(int i = 0; i < LIBS_FUNC.size();i++){
         Function * new_lib_func = new Function;
 
         new_lib_func->name = LIBS_FUNC[i];
         new_lib_func->scope = 0;
         new_lib_func->line = 0;
         new_lib_func->offset = 0;
 
         SymbolTableEntry_T entry = new SymbolTableEntry;
         entry->isActive = true;
         entry->value.funcVal = new_lib_func;
         entry->type = LIBFUNC;
 
         SymTable_put(oSymTable,LIBS_FUNC[i],entry);
         add_entry(scopelist,entry);
     }
 
}

int search_LIBS_FUNC(std::string &name){

    for(int i=0; i <LIBS_FUNC.size();i++){
        if(name == LIBS_FUNC[i]){
            return 0;
        }
    }

    return 1; //if not found 
}
 
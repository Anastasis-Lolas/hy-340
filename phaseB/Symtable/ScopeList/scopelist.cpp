#include "scopelist.h"

void add_entry(ScopeList_T& scopeList, SymbolTableEntry_T entry, int scope) {
    assert(entry);

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
void reactivate_scope(ScopeList_T& scopeList, int scope) {
    assert(scope >= 0 && scope < scopeList.size());
    for (auto& entry : scopeList[scope]) {
        entry->isActive = true;
    }
    return;
}
void delete_scopelist(ScopeList_T& scopeList, int scope) {
    assert(scope >= 0 && scope < scopeList.size());
    scopeList[scope].clear();
    scopeList.shrink_to_fit();
    return;
}

ScopeList_T get_active_vector(ScopeList_T& scopeList) {
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

void init_LIBS_FUNC(ScopeList_T& scopeList, SymTable_T oSymTable) {
    const size_t libFuncSize = sizeof(LIBS_FUNC) / sizeof(LIBS_FUNC[0]);
    for (int i = 0; i < libFuncSize; i++) {
        Function* new_lib_func = new Function;
        new_lib_func->name = LIBS_FUNC[i];
        new_lib_func->scope = 0;
        new_lib_func->line = 0;
        new_lib_func->offset = 0;

        SymbolTableEntry_T entry = new SymbolTableEntry;
        entry->isActive = true;
        entry->value.funcVal = new_lib_func;
        entry->type = LIBFUNC;

        SymTable_put(oSymTable, LIBS_FUNC[i], entry);
        add_entry(scopeList, entry, 0);  // Fixed scopelist to scopeList
    }
}

int search_LIBS_FUNC(std::string& name) {
    const size_t libFuncSize = sizeof(LIBS_FUNC) / sizeof(LIBS_FUNC[0]);
    for (int i = 0; i < libFuncSize; i++) {
        if (name == LIBS_FUNC[i]) {
            return 0;
        }
    }
    return 1;  // Not found
}


int find_offset(const ScopeList_T& scopeList, int scope) {
    if (scope < 0 || scope >= scopeList.size()) {
        return -1;
    }

    const auto& entries = scopeList[scope];
    if (entries.empty()) {
        return 0;
    }

    return entries.back()->value.varVal->offset;
}

SymbolTableEntry_T lookup_within_scope(const ScopeList_T& scopeList,
                                       const std::string& id, int scope) {
    if (scope < 0) {
        std::cout << "Error: Invalid scope " << scope << std::endl;
        return nullptr;
    } else if (scope >= static_cast<int>(scopeList.size())) {
        return nullptr;
    }

    for (const auto& entry : scopeList[scope]) {
        if ((entry->type == USERFUNC || entry->type == LIBFUNC)
                ? entry->value.funcVal->name == id
                : entry->value.varVal->name == id) {
            return entry;
        }
    }
    return nullptr;
}

SymbolTableEntry_T lookup_within_scope(const ScopeList_T& scopeList,
                                       const std::string& id, int scope,
                                       bool active) {
    if (scope < 0) {
        std::cout << "Error: Invalid scope " << scope << std::endl;
        return nullptr;
    } else if (scope >= static_cast<int>(scopeList.size())) {
        return nullptr;
    }
    for (const auto& entry : scopeList[scope]) {
        if (entry->isActive == active) {
            if ((entry->type == USERFUNC || entry->type == LIBFUNC)
                    ? entry->value.funcVal->name == id
                    : entry->value.varVal->name == id) {
                return entry;
            }
        }
    }
    return nullptr;
}

SymbolTableEntry_T lookup_active(const ScopeList_T& scopeList,
                                 const std::string& id, int scope) {
    if (scope < 0) {
        std::cout << "Error: Invalid scope " << scope << std::endl;
        return nullptr;
    }

    for (int i = scope; i >= 0; i--) {
        SymbolTableEntry_T entry = lookup_within_scope(scopeList, id, i, true);
        if (entry) return entry;
    }
    return nullptr;
}


void print_scopeList(ScopeList_T& scopeList) {
    std::cout << "========================================================\n";
    std::cout << "                     SCOPE LIST\n";
    std::cout << "========================================================\n";

    for (size_t scope = 0; scope < scopeList.size(); scope++) {
        if (scopeList[scope].empty()) continue;

        std::cout << "Scope " << scope << ":\n";

        for (const auto& entry : scopeList[scope]) {
            std::cout << "  Name: "
                      << ((entry->type == USERFUNC || entry->type == LIBFUNC)
                              ? entry->value.funcVal->name
                              : entry->value.varVal->name)
                      << " | Line: "
                      << ((entry->type == USERFUNC || entry->type == LIBFUNC)
                              ? entry->value.funcVal->line
                              : entry->value.varVal->line)
                      << " | Type: ";

            switch (entry->type) {
                case GLOBAL:
                    std::cout << "GLOBAL";
                    break;
                case LLOCAL:
                    std::cout << "LLOCAL";
                    break;
                case FORMAL:
                    std::cout << "FORMAL";
                    break;
                case USERFUNC:
                    std::cout << "USERFUNC";
                    break;
                case LIBFUNC:
                    std::cout << "LIBFUNC";
                    break;
                default:
                    std::cout << "UNKNOWN";
                    break;
            }

            std::cout << " | Active: " << (entry->isActive ? "Yes" : "No")
                      << "\n";
        }
    }

    std::cout << "========================================================\n";
}

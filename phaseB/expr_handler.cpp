#ifndef HANDLER_HEADER
#define HANDLER_HEADER
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"

unsigned int scope = 0;
unsigned int func_num = 0;
SymTable_T oSymTable;
extern int yylineno;
ScopeList_T scopeList;


void init_tables() {
    oSymTable = SymTable_new();
    init_LIBS_FUNC(scopeList, oSymTable);
}

std::string create_func_name(void) { return "_f" + std::to_string(func_num++); }

void exit_block() {
    scope_nodes_remove(scopeList, scope);
    scope--;
    reactivate_scope(scopeList, scope);
}
void add_function(std::string name, std::vector<void*> args) {
    SymbolTableEntry_T entry = nullptr;
    int offset;
    if (name == "") {
        name = create_func_name();
    } else {
        if (search_LIBS_FUNC(name) == 0) {
            // print error message shadows lib function
            std::cout << "Error: function " << name
                      << " shadows lib function at line " << yylineno
                      << std::endl;
            return;

        } else {
            entry = lookup_within_scope(scopeList, name, scope);
            if (entry) {
                if (entry->type == USERFUNC) {
                    // print error message function already declared in the same
                    // scope
                    std::cout << "Error: function " << name
                              << " already declared in line "
                              << entry->value.funcVal->line << std::endl;
                    return;
                } else {
                    // print error message function-same name as variable
                    std::cout << "Error: function " << name
                              << " same name as variable in line "
                              << entry->value.varVal->line << std::endl;
                }
                return;
            }
        }
    }
    offset = find_offset(scopeList, scope);

    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);
    add_entry(scopeList, entry, scope);

    SymTable_put(oSymTable, name, entry);
}

SymbolTableEntry_T add_ident(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    void* result;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    if (search_LIBS_FUNC(name) == 0) {
        // print error message shadows lib function
        std::cout << "Error: var with name: " << name
                  << " shadows lib function at line " << yylineno << std::endl;
        return nullptr;
    }
    entry = lookup_active(scopeList, name, scope);
    if (entry) {
        /* Found something (var or function) that we have access to it, so the
         * var refers to it */
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";
    } else {
        /* result = SymTable_general_lookup(oSymTable, name);
        entry = static_cast<SymbolTableEntry_T>(result);
        if (entry) {
            if (entry->type == USERFUNC) {
                // Collision with library function
                std::cout << "Error: Cannot access function :" << name
                          << " with scope: " << entry->value.funcVal->scope
                          << std::endl;
            } else {
                // print error message function-same name as variable
                std::cout << "Error: Cannot access variable with name: " << name
                          << " and scope: " << entry->value.varVal->scope
                          << std::endl;
            }
            return;
        } else {*/
        offset = find_offset(scopeList, scope);
        entry = SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
        add_entry(scopeList, entry, scope);
        SymTable_put(oSymTable, name, entry);
        // }
    }
    return entry;
}

SymbolTableEntry_T add_local_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    if (search_LIBS_FUNC(name) == 0 && symtype == LLOCAL) {
        // print error message shadows lib function
        std::cout << "Error: var with name: " << name
                  << " shadows lib function at line " << yylineno << std::endl;
        return nullptr;
    }
    entry = lookup_within_scope(scopeList, name, scope);
    if (entry) {
        // ref to the same var
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";
    } else {
        offset = find_offset(scopeList, scope);
        entry = SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
        add_entry(scopeList, entry, scope);
        SymTable_put(oSymTable, name, entry);
    }
    return entry;
}

SymbolTableEntry_T handle_namespace_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    entry = lookup_within_scope(scopeList, name, 0, true);
    if (!entry) {
        std::cout << "No global var of function with name: " << name
                  << std::endl;
    } else {
        // action for global var or function
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";
    }
    return entry;
}


void null_entry(SymbolTableEntry_T entry, std::string message) {
    if (!entry) {
        std::cout << "Error: '" << message
                  << "' was not declared in this scope (Line: " << yylineno
                  << ", Scope: " << scope << ")." << std::endl;
    }
}

void assign_error(SymbolTableEntry_T entry) {
    null_entry(entry, "lvalue for assignment ");
    if (entry) {
        if (entry->type == USERFUNC || entry->type == LIBFUNC) {
            std::cout << "Error: assigning value to a function'"
                      << " in this scope (Line: " << yylineno
                      << ", Scope: " << scope << ")." << std::endl;
        }
    }
}

void member_error(SymbolTableEntry_T entry, std::string memrule) {
    null_entry(entry, "lvalue ");
    if (entry) {
        if (entry->type == USERFUNC || entry->type == LIBFUNC) {
            std::cout << "Error: using function name as lvalue '" << memrule
                      << " in this scope (Line: " << yylineno
                      << ", Scope: " << scope << ")." << std::endl;
        }
    }
}
void print_entry(SymbolTableEntry_T entry) {
    if (!entry) {
        std::cout << "Entry is null" << std::endl;
        return;
    }
    std::cout << "Entry type: " << entry->type
              << " isActive: " << entry->isActive << std::endl;

    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        std::cout << "Function name: " << entry->value.funcVal->name
                  << ", Scope: " << entry->value.funcVal->scope
                  << ", Line: " << entry->value.funcVal->line << std::endl;
    } else {
        std::cout << "Variable name: " << entry->value.varVal->name
                  << ", Scope: " << entry->value.varVal->scope
                  << ", Line: " << entry->value.varVal->line << std::endl;
    }
}

void temrs_error(SymbolTableEntry_T entry, std::string op) {
    null_entry(entry, "lvalue ");

    if (entry) {
        if (entry->type == USERFUNC || entry->type == LIBFUNC) {
            std::cout << "Error: Cannot apply operator '" << op
                      << "' to a function '"
                      << "' (Line: " << yylineno << ", Scope: " << scope << ")."
                      << " Functions are constants and cannot be modified."
                      << std::endl;
        }
    }
}


std::vector<void *> handle_func_args(std::vector<void *> args,std::string name){

    // take the ident an check for duplicate in libs first
    if (search_LIBS_FUNC(name) == 0) {
        // print error message shadows lib function
        std::cout << "Error: var with name: " << name
                  << " shadows lib function at line " << yylineno << std::endl;
        return {};
    }

    for (void* arg : args) {
        std::string* cur_arg = static_cast<std::string*>(arg);
        if (*cur_arg == name) {
            std::cerr << "Error: duplicate argument '" << name 
                      << "' in function at line " << yylineno << std::endl;
            return {};
        }
    }
    
    std::string* name_copy = new std::string(name);
    args.push_back(static_cast<void*>(name_copy));

    // Add to symbol table
    int offset = find_offset(scopeList, scope);
    SymbolTableEntry_T formal_arg = SymTableEntry_new(
        FORMAL, name, scope, yylineno, offset, {}
    );
    add_entry(scopeList, formal_arg, scope);
    SymTable_put(oSymTable, name, formal_arg);

    return args;
}

void print_args(std::vector<void *> args){
      for (void* ptr : args) {
        std::string* str = static_cast<std::string*>(ptr);
        std::cout << *str << " ";
    }
    std::cout << std::endl;
}

void printFullSymTable(SymTable_T table) {
    std::map<unsigned int, std::vector<SymbolTableEntry_T>> scopedEntries;

    for (size_t i = 0; i < table->size; ++i) {
        hash_t node = table->buckets[i];
        while (node) {
            SymbolTableEntry_T entry =
                static_cast<SymbolTableEntry_T>(node->value);
            if (entry && entry->isActive) {
                unsigned int scope =
                    (entry->type == USERFUNC || entry->type == LIBFUNC)
                        ? entry->value.funcVal->scope
                        : entry->value.varVal->scope;
                scopedEntries[scope].push_back(entry);
            }
            node = node->next;
        }
    }

    for (const auto& [scope, entries] : scopedEntries) {
        if (entries.empty()) continue;

        std::cout << "------------   Scope #" << scope << "   ------------\n";


        std::vector<SymbolTableEntry_T> sortedEntries = entries;
        std::sort(
            sortedEntries.begin(), sortedEntries.end(),
            [](SymbolTableEntry_T a, SymbolTableEntry_T b) {
                unsigned int lineA = (a->type == USERFUNC || a->type == LIBFUNC)
                                         ? a->value.funcVal->line
                                         : a->value.varVal->line;
                unsigned int lineB = (b->type == USERFUNC || b->type == LIBFUNC)
                                         ? b->value.funcVal->line
                                         : b->value.varVal->line;
                return lineA < lineB;
            });

        for (const auto& entry : sortedEntries) {
            std::string name;
            std::string label;
            unsigned int line;

            switch (entry->type) {
                case GLOBAL:
                    label = "[global variable]";
                    break;
                case LLOCAL:
                    label = "[local variable]";
                    break;
                case FORMAL:
                    label = "[formal argument]";
                    break;
                case USERFUNC:
                    label = "[user function]";
                    break;
                case LIBFUNC:
                    label = "[library function]";
                    break;
                default:
                    label = "[unknown]";
                    break;
            }

            name = (entry->type == USERFUNC || entry->type == LIBFUNC)
                       ? entry->value.funcVal->name
                       : entry->value.varVal->name;

            line = (entry->type == USERFUNC || entry->type == LIBFUNC)
                       ? entry->value.funcVal->line
                       : entry->value.varVal->line;

            std::cout << "\"" << name << "\" " << label << " (line " << line
                      << ")"
                      << " (scope " << scope << ")\n";
        }
    }

    std::cout << "--------------------------------------------------\n";
}



#endif

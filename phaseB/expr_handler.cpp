#ifndef HANDLER_HEADER
#define HANDLER_HEADER
#include <iostream>
#include <vector>

#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"

unsigned int scope = 0;
unsigned int func_num = 0;
SymTable_T oSymTable;
int yylineno;
ScopeList_T scopeList;


void init_tables() {
    oSymTable = SymTable_new();
    init_LIBS_FUNC(scopeList, oSymTable);
}

std::string create_func_name(void) { return "_f" + std::to_string(func_num++); }


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

void add_ident(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    void* result;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    entry = lookup_active(scopeList, name, scope);
    if (entry) {
        /* Found something (var or function) that we have access to it, so the
         * var refers to it */
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";
    } else {
        result = SymTable_general_lookup(oSymTable, name);
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
        } else {
            offset = find_offset(scopeList, scope);
            entry =
                SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
            add_entry(scopeList, entry, scope);
            SymTable_put(oSymTable, name, entry);
        }
    }
}

void add_local_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    if (search_LIBS_FUNC(name) == 0 && symtype == LLOCAL) {
        // print error message shadows lib function
        std::cout << "Error: var with name: " << name
                  << " shadows lib function at line " << yylineno << std::endl;
        return;
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
}

void handle_namespace_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    entry = lookup_within_scope(scopeList, name, scope, true);
    if (!entry) {
        std::cout << "No global var of function with name: " << name
                  << std::endl;
    } else {
        // action for global var or function
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";
    }
    return;
}



void check_mutable_lvalue(SymbolTableEntry_T entry, const std::string& op) {
    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        std::string name = (entry->type == USERFUNC || entry->type == LIBFUNC)
            ? entry->value.funcVal->name
            : entry->value.varVal->name;

        std::cerr << "Error: cannot apply operator '" << op
                  << "' to function '" << name
                  << "' at line " << yylineno << std::endl;
    }
}

void check_assignable(SymbolTableEntry_T entry) {
    if (!entry) return;

    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        std::string name = (entry->type == USERFUNC || entry->type == LIBFUNC)
            ? entry->value.funcVal->name
            : entry->value.varVal->name;

        std::cerr << "Error: function '" << name
                  << "' cannot be used as lvalue at line "
                  << yylineno << std::endl;
    }
}



int main() {
    int arg1 = 42;
    std::string arg2 = "Hello";
    std::vector<void*> args = {&arg1, &arg2};
    SymbolTableEntry_T entry = nullptr;
    init_tables();

    add_ident("x");
    add_ident("y");
    scope++;
    add_ident("x");
    add_ident("error");
    scope++;
    deactivate_scope(scopeList, scope - 1);
    add_ident("error");
    print_scopeList(scopeList);
    // std::cout << "\n";
    // SymTable_print(oSymTable);
    // std::cout << "\n";

    std::cout << "================LOOK UP FOR X==============\n";

    // entry = lookup_active(scopeList, "x", scope);
    // std::cout << "Found entry: " << entry->value.varVal->name << " in scope "
    //           << entry->value.varVal->scope << std::endl;
    // scope--;
    reactivate_scope(scopeList, scope - 1);
    scope--;
    std::cout << "scope value is :" << scope << std::endl;
    entry = lookup_active(scopeList, "error", scope);
    if (entry) {
        std::cout << "ENTRY NOT NULL???\n";
        std::cout << "Found entry: " << entry->value.varVal->name
                  << " in scope " << entry->value.varVal->scope << std::endl;
    } else
        std::cout << "Entry not found var" << std::endl;
    // std::cout <<
    // "========================================================\n";
    // print_scopeList(scopeList);



    std::cout << "\n================INCREMENT TEST================\n";

    // Simulate valid ++x
    entry = lookup_active(scopeList, "x", scope);
    if (entry) {
        std::cout << "Testing ++x\n";
        check_mutable_lvalue(entry, "++");
        std::cout << "PASS: ++x is allowed (variable)\n";
    }

   
    scope = 0; // functions declared at global scope
    std::vector<void*> emptyArgs;
    add_function("myFunc", emptyArgs);

    // Lookup function and test ++
    entry = lookup_active(scopeList, "myFunc", scope);
    if (entry) {
        std::cout << "Testing ++myFunc (should fail)...\n";
        check_mutable_lvalue(entry, "++");  // errorrr
        std::cout << "FAIL: ++myFunc should not be allowed\n";
    }

    
    std::cout << "\n================ASSIGNMENT TEST================\n";

    // Simulate x = 5;
    entry = lookup_active(scopeList, "x", scope);
    if (entry) {
        std::cout << "Testing x = 5\n";
        check_assignable(entry);
        std::cout << "PASS: x = 5 is allowed (variable)\n";
    }

    // Simulate myFunc = 10; 
    entry = lookup_active(scopeList, "myFunc", scope);
    if (entry) {
        std::cout << "Testing myFunc = 10 (should fail)...\n";
        check_assignable(entry); // error
        std::cout << "FAIL: myFunc = 10 should not be allowed\n";
    }
    
    return 0;
}


#endif

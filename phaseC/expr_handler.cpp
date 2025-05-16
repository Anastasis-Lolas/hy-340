#ifndef HANDLER_HEADER
#define HANDLER_HEADER
#include "expr_handler.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"
#include "Quads/quad.h"

unsigned int scope = 0;
unsigned int func_num = 0;
unsigned int temp_num = 0;

SymTable_T oSymTable;
extern int yylineno;
ScopeList_T scopeList;
std::vector<int> scopeoffsetstack;
std::vector<std::string> anonym_funcs;
std::vector<unsigned> jump_stack;

extern unsigned programVarOffset;
extern unsigned functionLocalOffset;
extern unsigned formalArgOffset;
extern unsigned scopeSpaceCounter;


void normal_call(SymbolTableEntry_T entry) {
    std::string name;
    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        name = entry->value.funcVal->name;
    } else {
        name = entry->value.varVal->name;
    }
    SymbolTableEntry_T resu = nullptr;
    resu = lookup_in_list(scopeList, name, scope);
    if (resu == nullptr) {
        std::cerr << "Error at line " << yylineno << ": "
                  << "Identifier '" << name
                  << "' is not a function or is not accessible in scope "
                  << scope << "." << std::endl;
    }
}
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
void enter_func(int flag, std::string name) {
    scope++;
    // nextquad ??
    if (flag == 0) {
        // std::cout << "Anonymous function" << std::endl;
        name = create_func_name();
        anonym_funcs.push_back(name);
    } else {
        // std::cout << "Function name: " << name << std::endl;
    }
    // $funcprefix.iaddress = nextquadlabel();
    // unsigned funcStartQuad = nextquadlabel();
    jump_stack.push_back(nextquad());
    emit(jump, nullptr, nullptr, nullptr, 0, nextquad());
    // logika anti gia yylineno thelei quads ??
    emit(funcstart, newexpr_conststring(name), nullptr, nullptr, 0, nextquad());

    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();
    resetformalargsoffset();
    deactivate_scope(scopeList, scope);
}
void exit_func(int flag, std::string name, int returnList) {
    int offset, totalLocals;
    SymbolTableEntry_T entry = nullptr;
    if (flag == 0) {
        // std::cout << "Anonymous function" << std::endl;
        name = anonym_funcs.back();
        anonym_funcs.pop_back();
    }
    /*
    std::cout << "exit_" << name << " {" << std::endl;
    print_offset();
    std::cout << "}" << std::endl;
    */
    totalLocals = currscopeoffset();

    // pou to apothikeuw? entry->value.funcVal->totalLocals = totalLocals;
    offset = scopeoffsetstack.back();
    scopeoffsetstack.pop_back();
    resetfunctionlocaloffset();
    exitscopespace();
    restorecurrscopeoffset(offset);
    entry = lookup_within_scope(scopeList, name, scope);
    if (entry) {
        entry->value.funcVal->totalLocals = totalLocals;
        // std::cout << "Total local var: " << totalLocals << std::endl;
    }
    if (currscopespace() == formalarg) {
        exitscopespace();
    }
    emit(funcend, newexpr_conststring(name), nullptr, nullptr, 0, nextquad());
    patchlabel(jump_stack.back(), nextquad());
    jump_stack.pop_back();
    if (returnList) {
        patchlist(returnList, nextquad() - 1);
    }
}


void add_function(std::string name, std::vector<void*> args) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();

    resetfunctionlocaloffset();
    if (search_LIBS_FUNC(name) == 0) {
        // print error message shadows lib function
        std::cerr << "Error at line " << yylineno << ": function [" << name
                  << "] shadows lib function" << std::endl;
        return;

    } else {
        entry = lookup_within_scope(scopeList, name, scope);
        if (entry) {
            if (entry->type == USERFUNC) {
                // print error message function already declared in the same
                // scope
                std::cerr << "Error at line " << yylineno << ": function "
                          << name << " already declared in line "
                          << entry->value.funcVal->line << std::endl;
                return;
            } else {
                // print error message function-same name as variable
                std::cerr << "Error at line " << yylineno << ": function ["
                          << name << "] same name as variable in line "
                          << entry->value.varVal->line << std::endl;
            }
            return;
        }
    }

    // offset = find_offset(scopeList, scope);
    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);
    add_entry(scopeList, entry, scope);

    SymTable_put(oSymTable, name, entry);
}
void add_anon_function(std::vector<void*> args) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    std::string name = anonym_funcs.back();
    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();
    resetfunctionlocaloffset();

    // offset = find_offset(scopeList, scope);
    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);
    add_entry(scopeList, entry, scope);
    SymTable_put(oSymTable, name, entry);
}
SymbolTableEntry_T add_ident(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);

    entry = lookup_active(scopeList, name, scope);
    if (entry) {
        /* Found something (var or function) that we have access to it, so
        the
         * var refers to it
        std::cout << "Found something (var or function) with name: " << name
                  << " that we have access to "
                     "it, so the var refers to it\n";*/
        return entry;

    } else {
        entry = lookup_in_list(scopeList, name, scope);
        if (entry) {
            if (entry->type == USERFUNC) {
                // Collision with library function
                /*
                std::cout << "Error at line " << yylineno
                          << ": Cannot access function :" << name
                          << " with scope: " << entry->value.funcVal->scope
                          << " inside scope: " << scope << std::endl;*/
                return entry;
            } else {
                // print error message function-same name as variable
                std::cout << "Error at line " << yylineno
                          << ": Cannot access variable with name: " << name
                          << " and scope: " << entry->value.varVal->scope
                          << " inside scope: " << scope << std::endl;
            }
            return nullptr;
        } else {
            entry = lookup_within_scope(scopeList, name, 0, true);
            if (!entry) {
                // offset = find_offset(scopeList, scope);
                offset = currscopeoffset();
                // std::cout << "Creating new entry with name: " << name
                //           << " and offset: " << offset << std::endl;
                entry = SymTableEntry_new(symtype, name, scope, yylineno,
                                          offset, {});
                add_entry(scopeList, entry, scope);
                SymTable_put(oSymTable, name, entry);
                incurrscopeoffset();
            }
        }
    }

    return entry;
}
SymbolTableEntry_T add_local_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    if (search_LIBS_FUNC(name) == 0 && symtype == LLOCAL) {
        // print error message shadows lib function
        std::cerr << "Error at line " << yylineno << ": var with name [" << name
                  << "] shadows lib function " << std::endl;
        return nullptr;
    }
    entry = lookup_within_scope(scopeList, name, scope);

    // offset = find_offset(scopeList, scope);
    offset = currscopeoffset();
    entry = SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
    add_entry(scopeList, entry, scope);
    SymTable_put(oSymTable, name, entry);
    incurrscopeoffset();

    return entry;
}

SymbolTableEntry_T handle_namespace_dent(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    entry = lookup_within_scope(scopeList, name, 0, true);
    if (!entry) {
        std::cerr << "Error at line " << yylineno
                  << ": No global var of function with name: " << name
                  << std::endl;
    }
    return entry;
}


void null_entry(expr* entry, std::string message) {
    if (!entry) {
        std::cerr << "Error at line " << yylineno << ": " << message
                  << " was not declared in this scope (Scope: " << scope << ")."
                  << std::endl;
    }
}

void assign_error(expr* entry) {
    if (entry) {
        if (entry->type == programfunc_e || entry->type == libraryfunc_e) {
            std::cerr << "Error at line " << yylineno
                      << ": assigning value to a function'"
                      << " in this scope (Scope: " << scope << ")."
                      << std::endl;
        }
    }
}

void member_error(expr* entry, std::string memrule) {
    null_entry(entry, "lvalue ");
    if (entry) {
        if (entry->type == programfunc_e || entry->type == libraryfunc_e) {
            std::cerr << "Error at line " << yylineno
                      << ": using function name as lvalue '" << memrule
                      << " in this scope (Scope: " << scope << ")."
                      << std::endl;
        }
    }
}
void print_entry(SymbolTableEntry_T entry) {
    if (!entry) {
        std::cerr << "Error at line " << yylineno << ":Entry is null"
                  << std::endl;
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

void temrs_error(expr* entry, std::string op) {
    // null_entry(entry, "lvalue ");

    if (entry) {
        if (entry->type == programfunc_e || entry->type == libraryfunc_e) {
            std::cerr << "Error at line " << yylineno
                      << ": Cannot apply operator '" << op
                      << "' to a function (Scope: " << scope << ")."
                      << " Functions are constants and cannot be modified."
                      << std::endl;
        }
    }
}

int find_line(std::string name) {
    for (size_t i = 0; i < oSymTable->size; i++) {
        hash_t node = oSymTable->buckets[i];
        while (node) {
            SymbolTableEntry_T entry =
                static_cast<SymbolTableEntry_T>(node->value);

            if (entry && entry->type == FORMAL &&
                entry->value.varVal->name == name) {
                return entry->value.varVal->line;
            }

            node = node->next;
        }
    }

    return 0;
}

std::vector<void*> handle_func_args(std::vector<void*> args, std::string name) {
    // take the ident an check for duplicate in libs first
    if (search_LIBS_FUNC(name) == 0) {
        // print error message shadows lib function
        std::cerr << "Error at line " << yylineno << ": var with name: " << name
                  << " shadows lib function " << std::endl;
        return {};
    }

    for (void* arg : args) {
        int different_line = find_line(name);
        std::string* cur_arg = static_cast<std::string*>(arg);
        if (*cur_arg == name && different_line == yylineno) {
            std::cerr << "Error at line " << yylineno
                      << ": duplicate argument '" << name << "' in function"
                      << std::endl;
            return {};
        }
    }

    std::string* name_copy = new std::string(name);
    args.push_back(static_cast<void*>(name_copy));

    // Add to symbol table
    // int offset = find_offset(scopeList, scope);
    int offset = currscopeoffset();

    SymbolTableEntry_T formal_arg =
        SymTableEntry_new(FORMAL, name, scope, yylineno, offset, {});
    add_entry(scopeList, formal_arg, scope);
    SymTable_put(oSymTable, name, formal_arg);
    incurrscopeoffset();

    return args;
}

void print_args(std::vector<void*> args) {
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
            if (entry) {
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

        std::cout << "\n------------   Scope #" << scope << "   ------------\n";


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
            unsigned int offset;

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
            offset = (entry->type == USERFUNC || entry->type == LIBFUNC)
                         ? 0
                         : entry->value.varVal->offset;
            std::cout << "\"" << name << "\" " << label << " (line " << line
                      << ")"
                      << " (scope " << scope << ") (offset " << offset << ")\n";
        }
    }

    std::cout << "\n--------------------------------------------------\n";
}

// ===================================================================================

std::string newtempname() { return "_t" + std::to_string(temp_num++); }

void resettemp() { temp_num = 0; }

// flag ----?>>?>?>?>?>?>?
SymbolTableEntry_T newtemp() {
    // ignore for now
    SymbolTableEntry_T entry = nullptr;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    std::string name = newtempname();
    entry = lookup_within_scope(scopeList, name, scope);
    if (!entry) {
        // offset = find_offset(scopeList, scope);
        // offset = currscopeoffset();
        offset = currscopeoffset();
        entry = SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
        add_entry(scopeList, entry, scope);
        SymTable_put(oSymTable, name, entry);
        incurrscopeoffset();
    }
    return entry;
}

// expr* lvalue_id_handler(expr* lvalue, std::string name) {
//     expr* tableitem;

//     if (!lvalue) {
//         std::cerr << "Error in line " << yylineno
//                   << ": lvalue was not declared '" << name << "' in scope: ["
//                   << scope << "]." << std::endl;

//     } else if (lvalue->type == programfunc_e || lvalue->type ==
//     libraryfunc_e) {
//         std::cerr << "Error in line " << yylineno
//                   << ": Cannot use function name as lvalue '" << name
//                   << "' in scope: [" << scope << "]." << std::endl;
//     }
//     tableitem = member_item(lvalue, name);
//     return tableitem;
// }


expr* lvalue_id_handler(expr* lvalue, std::string name) {
    if (!lvalue) {
        std::cerr << "Error in line " << yylineno
                  << ": lvalue was not declared '" << name << "' in scope: ["
                  << scope << "]." << std::endl;
        return NULL;
    }

    if (lvalue->type == programfunc_e || lvalue->type == libraryfunc_e) {
        std::cerr << "Error in line " << yylineno
                  << ": Cannot use function name as lvalue '" << name
                  << "' in scope: [" << scope << "]." << std::endl;
        return NULL;
    }

    return member_item(lvalue, name);
}

expr* member_handler(expr* lvalue, expr* i) {
    lvalue = emit_iftableitem(lvalue);
    if (!lvalue) {
        std::cerr << "[DEBUG] NULL lvalue at member_handler" << std::endl;
        return NULL;
    }

    expr* item = newexpr(tableitem_e);
    item->sym = lvalue->sym;
    item->index = i;
    return item;
}
expr* member_item(expr* lvalue, std::string name) {
    lvalue = emit_iftableitem(lvalue);
    if (!lvalue) {
        std::cerr << "[DEBUG] NULL lvalue at member_item" << std::endl;
        return NULL;
    }

    expr* item = newexpr(tableitem_e);
    item->sym = lvalue->sym;
    item->index = newexpr_conststring(name);
    return item;
}

expr* newexpr(expr_t t) {
    expr* e = new expr;
    memset(e, 0, sizeof(expr));
    e->type = t;
    return e;
}

expr* newexpr_conststring(std::string str) {
    expr* e = newexpr(conststring_e);
    e->strConst = str;
    return e;
}


expr* newexpr_bool(bool b) {
    expr* e = newexpr(constbool_e);
    e->boolConst = b;
    return e;
}


expr* emit_iftableitem(expr* e) {
    if (e->type != tableitem_e) {
        return e;
    } else {
        expr* result = newexpr(var_e);
        result->sym = newtemp();
        emit(tablegetelem, e, e->index, result, 0, yylineno);
        return result;
    }
}


expr* newexpr_constnum(double i) {
    expr* e = newexpr(constnum_e);

    e->numConst = i;

    return e;
}

expr* emit_arith_op(iopcode op, expr* e1, expr* e2) {
    expr* result = newexpr(arithexpr_e);
    result->sym = newtemp();
    emit(op, e1, e2, result, 0, yylineno);
    return result;
}


expr* emit_assign_expr(expr* lval, expr* rval) {
    emit(assign, rval, nullptr, lval, 0, yylineno);
    return lval;  // result of assignment is the lvalue
}

expr* emit_relop_op(iopcode op, expr* e1, expr* e2) {
    expr* result = newexpr(boolexpr_e);
    result->sym = newtemp();
    emit(op, e1, e2, result, 0, yylineno);
    return result;
}

expr* symEntr_to_expr(SymbolTableEntry_T entry) {
    expr* e;
    if (!entry) {
        std::cerr << "Error at line " << yylineno << ": entry is null"
                  << std::endl;
        return nullptr;
    }
    if (entry->type == USERFUNC || entry->type == LIBFUNC) {
        e = newexpr(entry->type == USERFUNC ? programfunc_e : libraryfunc_e);
    } else if (entry->type == GLOBAL || entry->type == LLOCAL ||
               entry->type == FORMAL) {
        e = newexpr(var_e);
    }


    e->sym = entry;
    return e;
}

stmt_t* stmt_list_handler(stmt_t* s1, stmt_t* s2) {
    assert(s1);
    assert(s2);
    stmt_t* result = new stmt_t();
    make_stmt(result);


    if(result && result->breakList && result->contList && result->returnList){
        result->breakList = mergelist(s1->breakList, s2->breakList);
        result->contList = mergelist(s1->contList, s2->contList);
        result->returnList = mergelist(s1->returnList, s2->returnList);
    }
    return result;
}

expr* boolify_expr(expr* e) {
    if (e->type != boolexpr_e) {
        return e; // Non-boolean, return as-is
    }
    
    expr* result = newexpr(var_e);
    result->sym = newtemp();

  
    unsigned true_quad = nextquad();
    emit(assign, newexpr_bool(true), NULL, result, 0, yylineno);


    unsigned jump_after_true = nextquad();
    emit(jump, NULL, NULL, NULL, 0, yylineno);

    //quad for assign false se result
    unsigned false_quad = nextquad();
    emit(assign, newexpr_bool(false), NULL, result, 0, yylineno);

    // Backpatch true list in assign true
    backpatch(e->truelist, true_quad);

    // Backpatch false list assign false
    backpatch(e->falselist, false_quad);

   
   backpatch({(int)jump_after_true}, nextquad());


    return result;
}

expr* to_boolexpr(expr* e) {
    if (e->type == boolexpr_e) {
        return e;
    }

    expr* result = newexpr(boolexpr_e);
    result->sym = newtemp();

    unsigned true_jump = nextquad();
    emit(if_eq, e, newexpr_bool(true), NULL, 0, yylineno);  // if e == true goto _
    unsigned false_jump = nextquad();
    emit(jump, NULL, NULL, NULL, 0, yylineno);  // unconditional jump

    result->truelist.push_back(true_jump);
    result->falselist.push_back(false_jump);

    return result;
}
// For testing purposes
expr* anonym_call(SymbolTableEntry_T entry, expr* args) {
    expr* func = newexpr(programfunc_e);
    func->sym = entry;
    return call_handler(func, args);
}

expr* call_handler(expr* e, expr* elist) {
    expr* func = emit_iftableitem(e);

    std::vector<expr*> args;
    for (expr* arg = elist; arg != nullptr; arg = arg->next) {
        args.push_back(arg);
    }

    // Stack-like access
    for (auto it = args.rbegin(); it != args.rend(); ++it) {
        emit(param, *it, nullptr, nullptr, 0, nextquadlabel());
    }
    emit(call, func, nullptr, nullptr, 0, nextquadlabel());

    expr* result = newexpr(var_e);
    result->sym = newtemp();
    emit(getretval, result, nullptr, result, 0, yylineno);
    return result;
}

call_t* normcall_handler(expr* e) {
    call_t* temp = new call_t;
    memset(temp, 0, sizeof(call_t));

    temp->elist = e;
    temp->method = 0;
    // temp->name = "";  // nullstring??
    return temp;
}
call_t* methodcall_handler(expr* e, std::string name) {
    call_t* temp = new call_t;
    memset(temp, 0, sizeof(call_t));

    temp->elist = e;
    temp->method = 1;
    temp->name = name;
    return temp;
}

// ignore for now=================================================
expr* normal_call_handler(std::vector<expr*> args) {             //|
    for (auto it = args.rbegin(); it != args.rend(); ++it) {     //|
        emit(param, *it, nullptr, nullptr, 0, nextquadlabel());  //|
    }  //|
    return nullptr;  //|
}  //|
//=================================================================

#endif

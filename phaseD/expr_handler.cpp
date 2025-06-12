#ifndef HANDLER_HEADER
#define HANDLER_HEADER
#include "expr_handler.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include "Quads/quad.h"
#include "Symtable/ScopeList/scopelist.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/symtable.h"

unsigned int scope = 0;
unsigned int func_num = 0;
unsigned int temp_num = 0;
int infunction = 0;

SymTable_T oSymTable;
extern int yylineno;
ScopeList_T scopeList;
std::vector<int> scopeoffsetstack;
std::vector<std::string> anonym_funcs;
std::vector<unsigned> jump_stack;
std::vector<int> lc_Vstack;

extern unsigned programVarOffset;
extern unsigned functionLocalOffset;
extern unsigned formalArgOffset;
extern unsigned scopeSpaceCounter;

int debug = 0;

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
    infunction++;
    if (flag == 0) {
        name = create_func_name();
        anonym_funcs.push_back(name);
    } else {
        // std::cout << "Function name: " << name << std::endl;
    }
    push_loopcounter();
    jump_stack.push_back(nextquad());
    emit(jump, nullptr, nullptr, nullptr, 0, nextquad());
    emit(funcstart, nullptr, nullptr, newexpr(programfunc_e), -1, nextquad());

    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();
    resetformalargsoffset();
    deactivate_scope(scopeList, scope);
}
SymbolTableEntry_T exit_func(int flag, std::string name, int returnList) {
    int offset, totalLocals;
    SymbolTableEntry_T entry = nullptr;
    if (flag == 0) {
        name = anonym_funcs.back();
        anonym_funcs.pop_back();
    }
    pop_loopcounter();
    totalLocals = currscopeoffset();

    offset = scopeoffsetstack.back();
    scopeoffsetstack.pop_back();
    resetfunctionlocaloffset();
    exitscopespace();
    restorecurrscopeoffset(offset);
    entry = lookup_within_scope(scopeList, name, scope);
    if (entry) {
        entry->value.funcVal->totalLocals = totalLocals;
    }
    if (currscopespace() == formalarg) {
        exitscopespace();
    }
    emit(funcend, nullptr, nullptr, newexpr_conststring(name), -1, nextquad());
    patchlabel(jump_stack.back(), nextquad());
    jump_stack.pop_back();
    if (returnList) {
        patchlist(returnList, nextquad() - 1);
    }
    infunction--;
    return entry;
}


void add_function(std::string name, std::vector<void*> args,
                  unsigned int funcIndex) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();

    resetfunctionlocaloffset();
    if (search_LIBS_FUNC(name) == 0) {
        std::cerr << "Error at line " << yylineno << ": function [" << name
                  << "] shadows lib function" << std::endl;
        return;

    } else {
        entry = lookup_within_scope(scopeList, name, scope);
        if (entry) {
            if (entry->type == USERFUNC) {
                std::cerr << "Error at line " << yylineno << ": function "
                          << name << " already declared in line "
                          << entry->value.funcVal->line << std::endl;
                return;
            } else {
                std::cerr << "Error at line " << yylineno << ": function ["
                          << name << "] same name as variable in line "
                          << entry->value.varVal->line << std::endl;
            }
            return;
        }
    }

    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);
    entry->value.funcVal->funcIndex = funcIndex;
    add_entry(scopeList, entry, scope);

    SymTable_put(oSymTable, name, entry);
}
void add_anon_function(std::vector<void*> args, unsigned int funcIndex) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    std::string name = anonym_funcs.back();
    scopeoffsetstack.push_back(currscopeoffset());
    enterscopespace();
    resetfunctionlocaloffset();

    entry = SymTableEntry_new(USERFUNC, name, scope, yylineno, offset, args);
    entry->value.funcVal->funcIndex = funcIndex;
    add_entry(scopeList, entry, scope);
    SymTable_put(oSymTable, name, entry);
}
SymbolTableEntry_T add_ident(std::string name) {
    SymbolTableEntry_T entry = nullptr;
    int offset = 0;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);

    entry = lookup_active(scopeList, name, scope);
    if (entry) {
        return entry;

    } else {
        entry = lookup_in_list(scopeList, name, scope);
        if (entry) {
            if (entry->type == USERFUNC) {
                // Collision with library function
                return entry;
            } else {
                // print error message function-same name as variable
                std::cerr << "Error at line " << yylineno
                          << ": Cannot access variable with name: " << name
                          << " and scope: " << entry->value.varVal->scope
                          << " inside scope: " << scope << std::endl;
            }
            return nullptr;
        } else {
            entry = lookup_within_scope(scopeList, name, 0, true);
            if (!entry) {
                offset = currscopeoffset();
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


std::string newtempname() { return "_t" + std::to_string(temp_num++); }

void resettemp() { temp_num = 0; }

SymbolTableEntry_T newtemp() {
    // ignore for now
    SymbolTableEntry_T entry = nullptr;
    int offset;
    SymbolType symtype = (scope == 0 ? GLOBAL : LLOCAL);
    std::string name = newtempname();
    // entry = lookup_within_scope(scopeList, name, scope);
    if (!entry) {
        offset = currscopeoffset();
        entry = SymTableEntry_new(symtype, name, scope, yylineno, offset, {});
        add_entry(scopeList, entry, scope);
        SymTable_put(oSymTable, name, entry);
        incurrscopeoffset();
    }
    return entry;
}


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
    if (!lvalue) {
        std::cerr << "[DEBUG] NULL lvalue at member_handler" << std::endl;
        return NULL;
    }
    lvalue = emit_iftableitem(lvalue);
    expr* item = newexpr(tableitem_e);
    item->sym = lvalue->sym;
    item->index = i;
    return item;
}
expr* member_item(expr* lvalue, std::string name) {
    if (!lvalue) {
        std::cerr << "[DEBUG] NULL lvalue at member_item" << std::endl;
        return NULL;
    }
    lvalue = emit_iftableitem(lvalue);
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
    // expr* e = newexpr2(conststring_e);
    expr* e = new expr();
    e->type = conststring_e;
    e->strConst = str;
    return e;
}


expr* newexpr_bool(bool b) {
    expr* e = newexpr(constbool_e);
    e->boolConst = b;
    return e;
}

expr* newexpr_nill() {
    expr* e = newexpr(nil_e);
    return e;
}

expr* emit_iftableitem(expr* e) {
    if (e->type != tableitem_e) {
        return e;
    } else {
        expr* result = newexpr(var_e);
        result->sym = newtemp();
        emit(tablegetelem, e, e->index, result, -1, yylineno);
        return result;
    }
}


expr* newexpr_constdouble(double i) {
    expr* e = newexpr(constdouble_e);
    e->sym = NULL;
    e->numConst = i;

    return e;
}

expr* newexpr_intnum(int i) {
    expr* e = newexpr(intnum_e);
    e->sym = NULL;
    e->numInt = i;
    return e;
}

expr* emit_arith_op(iopcode op, expr* e1, expr* e2) {
    expr* result = newexpr(arithexpr_e);
    result->sym = newtemp();
    emit(op, e1, e2, result, -1, yylineno);
    return result;
}


expr* emit_assign_expr(expr* lval, expr* rval) {
    emit(assign, rval, nullptr, lval, -1, yylineno);
    return lval;  // result of assignment is the lvalue
}

expr* emit_relop_op(iopcode op, expr* e1, expr* e2) {
    expr* result = newexpr(boolexpr_e);
    result->sym = newtemp();
    emit(op, e1, e2, result, -1, yylineno);
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

    if (result) {
        result->breakList = mergelist(s1->breakList, s2->breakList);
        result->contList = mergelist(s1->contList, s2->contList);
        result->returnList = mergelist(s1->returnList, s2->returnList);
    }

    return result;
}

expr* boolify_expr(expr* e) {
    if (e->type != boolexpr_e) {
        return e;  // Non-boolean, return as-is
    }

    expr* result = newexpr(var_e);
    result->sym = newtemp();


    unsigned true_quad = nextquad();
    emit(assign, newexpr_bool(true), NULL, result, -1, yylineno);


    unsigned jump_after_true = nextquad();
    emit(jump, NULL, NULL, NULL, 0, yylineno);

    // quad for assign false se result
    unsigned false_quad = nextquad();
    emit(assign, newexpr_bool(false), NULL, result, -1, yylineno);

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
    emit(if_eq, e, newexpr_bool(true), NULL, 0,
         yylineno);  // if e == true goto _
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
        emit(param, *it, nullptr, nullptr, -1, nextquadlabel());
    }
    emit(call, nullptr, nullptr, func, -1, nextquadlabel());

    expr* result = newexpr(var_e);
    result->sym = newtemp();
    emit(getretval, nullptr, nullptr, result, -1, yylineno);
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
void check_arith(expr* e, std::string op) {
    if (e->type == constbool_e || e->type == conststring_e ||
        e->type == nil_e || e->type == newtable_e || e->type == programfunc_e ||
        e->type == libraryfunc_e || e->type == boolexpr_e) {
        printf("Illegal expr used in %s\n", op.c_str());
    }
}

bool check_arithmetic_expr(expr* e) {
    assert(e);
    if (e->type == constbool_e || e->type == conststring_e ||
        e->type == nil_e || e->type == newtable_e || e->type == programfunc_e ||
        e->type == libraryfunc_e || e->type == boolexpr_e) {
        std::cerr << "ERROR Illegal expr  used in line " << yylineno
                  << std::endl;
        return false;
    }
    return true;
}

stmt_t* handle_breaks() {
    stmt_t* $$ = new stmt_t();
    make_stmt($$);

    if (loopcounter == 0) {
        std::cerr << "break statement is outside of the loop \n";
    } else {
        $$ = new stmt_t();
        make_stmt($$);
        $$->breakList = nextquad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
    return $$;
}

// debug
void print_expr_list(expr* head) {
    std::cout << "Debug print#" << debug++ << std::endl;
    int i = 0;
    while (head) {
        std::cout << "[" << i++ << "] ";
        print_expr(head);
        head = head->next;
    }
}

void print_expr(expr* e) {
    if (!e) {
        std::cout << "expr: null\n";
        return;
    }

    std::cout << "expr: {\n";

    std::cout << "  type: ";
    switch (e->type) {
        case var_e:
            std::cout << "var_e";
            break;
        case tableitem_e:
            std::cout << "tableitem_e";
            break;
        case programfunc_e:
            std::cout << "programfunc_e";
            break;
        case libraryfunc_e:
            std::cout << "libraryfunc_e";
            break;
        case arithexpr_e:
            std::cout << "arithexpr_e";
            break;
        case boolexpr_e:
            std::cout << "boolexpr_e";
            break;
        case assignexpr_e:
            std::cout << "assignexpr_e";
            break;
        case newtable_e:
            std::cout << "newtable_e";
            break;
        case constdouble_e:
            std::cout << "constdouble_e";
            break;
        case intnum_e:
            std::cout << "intnum_e";
            break;
        case constbool_e:
            std::cout << "constbool_e";
            break;
        case conststring_e:
            std::cout << "conststring_e";
            break;
        case nil_e:
            std::cout << "nil_e";
            break;
        default:
            std::cout << "unknown";
            break;
    }
    std::cout << "\n";

    std::cout << "  sym: ";
    if (e->sym) {
        if (e->sym->type == USERFUNC || e->sym->type == LIBFUNC) {
            std::cout << e->sym->value.funcVal->name;
        } else if (e->sym->value.varVal) {
            std::cout << e->sym->value.varVal->name;
        } else {
            std::cout << "(unnamed)";
        }
        std::cout << "\n";
    } else {
        std::cout << "null\n";
    }

    switch (e->type) {
        case intnum_e:
            std::cout << "  numConst: " << e->numConst << "\n";
            break;
        case constdouble_e:
            std::cout << "  numConst: " << e->numConst << "\n";
            break;
        case conststring_e:
            std::cout << "  strConst: \"" << e->strConst << "\"\n";
            break;
        case constbool_e:
            std::cout << "  boolConst: " << (e->boolConst ? "true" : "false")
                      << "\n";
            break;
        case tableitem_e:
            std::cout << "  table index:\n";
            print_expr(e->index);
            break;
        default:
            break;
    }

    if (e->next) {
        std::cout << "  next →\n";
        print_expr(e->next);
    }

    std::cout << "}\n";
}

#endif
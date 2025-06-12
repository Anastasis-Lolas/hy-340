#include "avm_helper.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_set>

#include "avm_table.h"

typedef bool (*tobool_func_t)(avm_memcell*);

typedef std::string (*tostring_func_t)(avm_memcell*);

tobool_func_t toboolFuncs[] = {number_tobool, string_tobool,   bool_tobool,
                               table_tobool,  userfunc_tobool, libfunc_tobool,
                               nil_tobool,    undef_tobool};


tostring_func_t toStringFunctions[] = {
    number_toString,   string_toString,  bool_toString, table_toString,
    userfunc_toString, libfunc_toString, nil_toString,  undef_toString};


bool number_tobool(avm_memcell* m) { return m->data.numVal != 0; }

bool string_tobool(avm_memcell* m) { return !(m->data.strVal.empty()); }

bool bool_tobool(avm_memcell* m) { return m->data.boolVal; }

bool table_tobool(avm_memcell*) { return true; }

bool userfunc_tobool(avm_memcell*) { return true; }

bool libfunc_tobool(avm_memcell*) { return true; }

bool nil_tobool(avm_memcell*) { return false; }

bool undef_tobool(avm_memcell*) {
    avm_warning("Trying to convert 'undefined' to boolean.");
    return false;
}

bool avm_tobool(avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}


std::string avm_toString(avm_memcell* m) {
    assert(m->type >= 0 && m->type <= undef_m);
    return (*toStringFunctions[m->type])(m);
}

std::string number_toString(avm_memcell* m) {
    assert(m && m->type == number_m);
    return std::to_string(m->data.numVal);
}

std::string string_toString(avm_memcell* m) {
    assert(m->type == string_m);
    return m->data.strVal;
}

std::string bool_toString(avm_memcell* m) {
    assert(m && m->type == bool_m);
    return m->data.boolVal ? "true" : "false";
}

/*
std::string table_toString(avm_memcell* m) {
    assert(m && m->type == table_m);

    avm_table* table = m->data.tableVal;
    assert(table);

    std::ostringstream oss;
    oss << "[ ";


    std::vector<double> keys;
    keys.reserve(table->numIndexed->size());
    for (const auto& p : *table->numIndexed) keys.push_back(p.first);
    std::sort(keys.begin(), keys.end());


    for (size_t i = 0; i < keys.size(); ++i) {
        double idx = keys[i];
        avm_memcell& cell = (*table->numIndexed)[idx];
        // if nested table, recurse; otherwise print directly
        if (cell.type == table_m) {
            oss << table_toString(&cell);
        } else {
            oss << avm_toString(&cell);
        }
        if (i + 1 < keys.size()) oss << ", ";
    }

    oss << " ]";
    return oss.str();
}
*/

std::string userfunc_toString(avm_memcell* m) {
    assert(m && m->type == userfunc_m);
    return "userfunc" + std::to_string(m->data.funcVal);
}

std::string libfunc_toString(avm_memcell* m) {
    assert(m && m->type == libfunc_m);
    return m->data.libfuncVal;
}
std::string nil_toString(avm_memcell* m) {
    assert(m && m->type == nil_m);
    return "nil";
}
std::string undef_toString(avm_memcell* m) {
    assert(m && m->type == undef_m);
    return "undefined";
}


void avm_error(const std::string& msg) {
    std::cerr << "\n\033[1;31mAVM Error: " << msg << "\033[0m\n";
}
void avm_warning(const std::string& msg) {
    std::cerr << "\n\033[1;33mAVM Warning: " << msg << "\033[0m\n";
}


std::string table_toString_recursive(avm_table* table,
                                     std::unordered_set<avm_table*>& visited);

std::string table_toString(avm_memcell* m) {
    assert(m && m->type == table_m);
    std::unordered_set<avm_table*> visited;
    return table_toString_recursive(m->data.tableVal, visited);
}

std::string table_toString_recursive(avm_table* table,
                                     std::unordered_set<avm_table*>& visited) {
    assert(table);

    if (visited.count(table)) {
        return "[...CYCLES...]";
    }
    visited.insert(table);

    std::ostringstream oss;
    oss << "[";
    bool first_element_printed = false;

    if (!table->numIndexed->empty()) {
        std::vector<double> keys;
        keys.reserve(table->numIndexed->size());
        for (const auto& pair : *table->numIndexed) {
            keys.push_back(pair.first);
        }
        std::sort(keys.begin(), keys.end());

        for (size_t i = 0; i < keys.size(); ++i) {
            if (i > 0) oss << ", ";
            avm_memcell& cell = (*table->numIndexed)[keys[i]];
            oss << " { " << keys[i] << " : ";
            if (cell.type == table_m) {
                oss << table_toString_recursive(cell.data.tableVal, visited);
            } else {
                oss << avm_toString(&cell);
            }
            oss << " }";
        }
        first_element_printed = true;
    }

    if (!table->strIndexed->empty()) {
        if (first_element_printed) {
            oss << ", ";
        }

        bool first_str_key = true;
        for (auto& pair : *table->strIndexed) {
            if (!first_str_key) oss << ", ";
            avm_memcell& cell = pair.second;
            oss << " { \"" << pair.first << "\" : ";
            if (cell.type == table_m) {
                oss << table_toString_recursive(cell.data.tableVal, visited);
            } else {
                oss << avm_toString(&cell);
            }
            oss << " }";
            first_str_key = false;
        }
    }

    oss << " ]";

    visited.erase(table);
    return oss.str();
}
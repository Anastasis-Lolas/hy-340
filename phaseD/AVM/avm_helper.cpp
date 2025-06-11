#include "avm_helper.h"
#include <sstream>
#include <iomanip>
#include "avm_table.h"

typedef bool (*tobool_func_t)(avm_memcell*);

typedef std::string (*tostring_func_t)(avm_memcell*);

tobool_func_t toboolFuncs[] = {number_tobool, string_tobool,   bool_tobool,
                               table_tobool,  userfunc_tobool, libfunc_tobool,
                               nil_tobool,    undef_tobool};


tostring_func_t toStringFunctions[] = {
    number_toString,   string_toString,  bool_toString, table_toString,
    userfunc_toString, libfunc_toString, nil_toString,  undef_toString};


bool number_tobool(avm_memcell* m) { return m->numVal != 0; }

bool string_tobool(avm_memcell* m) { return !(m->strVal.empty()); }

bool bool_tobool(avm_memcell* m) { return m->boolVal; }

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
    return std::to_string(m->numVal);
}

std::string string_toString(avm_memcell* m) {
    assert(m->type == string_m);
    return m->strVal;
}

std::string bool_toString(avm_memcell* m) {
    assert(m && m->type == bool_m);
     return m->boolVal ? "true" : "false";
}


std::string table_toString(avm_memcell* m) {
    assert(m && m->type == table_m);

    avm_table* table = m->tableVal;
    std::ostringstream oss;
    oss << "{ ";
    for (auto it = table->numIndexed->begin(); it != table->numIndexed->end(); ++it)
        oss << "[" << it->first << "] = " << avm_toString(&(it->second)) << ", ";

    for (auto it = table->strIndexed->begin(); it != table->strIndexed->end(); ++it)
        oss << "[\"" << it->first << "\"] = " << avm_toString(&(it->second)) << ", ";

    for (auto it = table->boolIndexed->begin(); it != table->boolIndexed->end(); ++it)
        oss << "[" << (it->first ? "true" : "false") << "] = " << avm_toString(&(it->second)) << ", ";

    for (auto it = table->tableIndexed->begin(); it != table->tableIndexed->end(); ++it)
        oss << "[table" << it->first << "] = " << avm_toString(&(it->second)) << ", ";

    for (auto it = table->userfuncIndexed->begin(); it != table->userfuncIndexed->end(); ++it)
        oss << "[userfunc" << it->first << "] = " << avm_toString(&(it->second)) << ", ";

    for (auto it = table->libfuncIndexed->begin(); it != table->libfuncIndexed->end(); ++it)
        oss << "[libfunc \"" << it->first << "\"] = " << avm_toString(&(it->second)) << ", ";

        
    std::string result = oss.str();
    if (result.size() > 2)
        result.erase(result.end() - 2, result.end()); // remove last ", "
    result += " }";
    return result;
}


std::string userfunc_toString(avm_memcell* m) {
    assert(m && m->type == userfunc_m);
    return "userfunc" + std::to_string(m->funcVal);
}

std::string libfunc_toString(avm_memcell* m) {
    assert(m && m->type == libfunc_m);
    return m->libfuncVal;
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
    std::cerr << "\033[1;31mAVM Error: " << msg << "\033[0m\n";
}
void avm_warning(const std::string& msg) {
    std::cerr << "\033[1;33mAVM Warning: " << msg << "\033[0m\n";
}

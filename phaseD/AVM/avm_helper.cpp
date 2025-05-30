#include "avm_helper.h"


bool number_tobool(avm_memcell* m) { return m->data.numVal != 0; }

bool string_tobool(avm_memcell* m) { return !(m->data.strVal.empty()); }

bool bool_tobool(avm_memcell* m) { return m->data.boolVal; }

bool table_tobool(avm_memcell* m) { return true; }

bool userfunc_tobool(avm_memcell* m) { return true; }

bool libfunc_tobool(avm_memcell* m) { return true; }

bool nil_tobool(avm_memcell* m) { return false; }

bool undef_tobool(avm_memcell* m) {
    assert(0);
    return false;
}

bool avm_tobool(avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

tobool_func_t toboolFuncs[] = {number_tobool, string_tobool,   bool_tobool,
                               table_tobool,  userfunc_tobool, libfunc_tobool,
                               nil_tobool,    undef_tobool};


tostring_func_t toStringFunctions[] = {
    number_toString,
    string_toString,
    bool_toString,
    table_toString,
    userfunc_toString,
    libfunc_toString,
    nil_toString,
    undef_toString
};    

std::string avm_toString (avm_memcell* m){
    assert(m->type >= 0 && m->type <= undef_m);
    return (*toStringFunctions[m->type]) (m);
}

std::string number_toString (avm_memcell* m){
    assert( m && m->type == number_m);
    return std::to_string(m->data.numVal);
}

std::string string_toString (avm_memcell* m){
    assert(m->type == string_m);
    return m->data.strVal;
}

std::string bool_toString (avm_memcell* m){
    assert(m && m->type == bool_m);
    bool b = m->data.boolVal;
    return b ? "true" : "false";
}


std::string table_toString (avm_memcell* m){
    assert(m && m->type == table_m);
    return "[table]not implemented yet";
}

std::string userfunc_toString (avm_memcell* m){
    assert(m && m->type == userfunc_m);
    return "userfunc" + std::to_string(m->data.funcVal);
}

std::string libfunc_toString (avm_memcell* m) {
    assert(m && m->type == libfunc_m);
    return m->data.libfuncVal;
}
std::string nil_toString (avm_memcell* m){
    assert(m && m->type == nil_m);
    return "nil";
}
std::string undef_toString (avm_memcell* m){
    assert(m && m->type == undef_m);
    return "undefined";
}


void avm_error(const std::string& msg) {
    std::cerr << "\033[1;31mAVM Error: " << msg << "\033[0m\n";
}
void avm_warning(const std::string& msg) {
    std::cerr << "\033[1;33mAVM Warning: " << msg << "\033[0m\n";
}

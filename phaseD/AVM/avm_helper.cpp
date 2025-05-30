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

void avm_error(const std::string& msg) {
    std::cerr << "\033[1;31mAVM Error: " << msg << "\033[0m\n";
}
void avm_warning(const std::string& msg) {
    std::cerr << "\033[1;33mAVM Warning: " << msg << "\033[0m\n";
}

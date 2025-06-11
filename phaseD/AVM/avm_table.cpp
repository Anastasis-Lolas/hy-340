#include "avm_table.h"

#include <cstring>  // For memset
#include <iostream>

#include "../t-codeLib/t-code.h"
#include "avm_helper.h"
#include "memcell_struct.h"


// Helper function to copy avm_memcell content
void avm_memcell_copy(avm_memcell* dest, const avm_memcell* src) {
    dest->type = src->type;
    switch (src->type) {
        case number_m:
            dest->data.numVal = src->data.numVal;
            break;
        case string_m:
            dest->data.strVal = src->data.strVal;
            break;
        case bool_m:
            dest->data.boolVal = src->data.boolVal;
            break;
        case table_m:
            dest->data.tableVal = src->data.tableVal;
            if (src->data.tableVal) {
                avm_tableincrefcounter(src->data.tableVal);
            }
            break;
        case userfunc_m:
            dest->data.funcVal = src->data.funcVal;
            break;
        case libfunc_m:
            dest->data.libfuncVal = src->data.libfuncVal;
            break;
        case nil_m:
        case undef_m:
            // No data to copy
            break;
        default:
            std::cerr << "Error: Invalid type in avm_memcell_copy."
                      << std::endl;
            break;
    }
}


void avm_tablebucketsinit(avm_table_bucket** buckets) {
    for (unsigned i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        buckets[i] = (avm_table_bucket*)0;
    }
}

avm_table::avm_table() {
    this->strIndexed = new std::unordered_map<std::string, avm_memcell>;
    this->numIndexed = new std::unordered_map<double, avm_memcell>;
    this->userfuncIndexed = new std::unordered_map<unsigned, avm_memcell>;
    this->libfuncIndexed = new std::unordered_map<std::string, avm_memcell>;
    this->boolIndexed = new std::unordered_map<bool, avm_memcell>;
    this->tableIndexed =
        new std::unordered_map<avm_table*, avm_memcell, std::hash<avm_table*>>;

    this->total = 0;
}
avm_table::~avm_table() {
    // Clean up all avm_memcell values in the maps
    for (auto& pair : *strIndexed) {
        avm_memcellclear(&pair.second);
    }
    for (auto& pair : *numIndexed) {
        avm_memcellclear(&pair.second);
    }
    for (auto& pair : *userfuncIndexed) {
        avm_memcellclear(&pair.second);
    }
    for (auto& pair : *libfuncIndexed) {
        avm_memcellclear(&pair.second);
    }
    for (auto& pair : *boolIndexed) {
        avm_memcellclear(&pair.second);
    }
    for (auto& pair : *tableIndexed) {
        avm_memcellclear(&pair.second);
        avm_tabledecrefcounter(
            pair.first);  // Decrement ref count for table keys
    }
    delete this->strIndexed;
    delete this->numIndexed;
    delete this->userfuncIndexed;
    delete this->libfuncIndexed;
    delete this->boolIndexed;
    delete this->tableIndexed;
}


void avm_tabledestroy(avm_table* t) { delete t; }


unsigned avm_table::get_avm_tableElement_count() {
    unsigned total = 0;
    total += this->strIndexed->size();
    total += this->numIndexed->size();
    total += this->userfuncIndexed->size();
    total += this->libfuncIndexed->size();
    total += this->boolIndexed->size();
    total += this->tableIndexed->size();

    this->total = total;
    return total;
}

avm_table* avm_tablenew(void) {
    avm_table* t = new avm_table();
    t->refCounter = 0; /* total becomes set inside ctor */
    // AVM_WIPEOUT(*t);

    return t;
}

void avm_tableincrefcounter(avm_table* t) {
    if (t) {
        t->refCounter++;
    } else {
        std::cerr << "Error: Attempt to increment refCounter of a null table."
                  << std::endl;
    }
}
void avm_tabledecrefcounter(avm_table* t) {
    // assert(t->refCounter > 0); // Ensure refCounter is > 0
    if (t->refCounter == 0) {
        std::cerr << "Warning: Attempt to decrement zero refCounter"
                  << std::endl;
        return;
    }
    if (!--t->refCounter) {
        avm_tabledestroy(t);
    }
}

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index) {
    assert(table != nullptr && index != nullptr);

    switch (index->type) {
        case number_m: {
            double key = index->data.numVal;
            auto it = table->numIndexed->find(key);
            if (it != table->numIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case string_m: {
            std::string key(index->data.strVal);
            auto it = table->strIndexed->find(key);
            if (it != table->strIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case bool_m: {
            bool key = index->data.boolVal;
            auto it = table->boolIndexed->find(key);
            if (it != table->boolIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case table_m: {
            avm_table* key = index->data.tableVal;
            auto it = table->tableIndexed->find(key);
            if (it != table->tableIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case userfunc_m: {
            unsigned key = index->data.funcVal;
            auto it = table->userfuncIndexed->find(key);
            if (it != table->userfuncIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case libfunc_m: {
            std::string key(index->data.libfuncVal);
            auto it = table->libfuncIndexed->find(key);
            if (it != table->libfuncIndexed->end()) {
                return &it->second;
            }
            break;
        }
        case nil_m: {
            // Can't index by nil
            avm_error("Cannot index table by nil!");
            break;
        }
        default:
            avm_error("Error: Invalid key type in avm_tablegetelem.\n");
            break;
    }
    return nullptr;  // Key not found
}


void avm_tablesetelem(avm_table* table, avm_memcell* index,
                      avm_memcell* content) {
    assert(table != nullptr && index != nullptr && content != nullptr);

    switch (index->type) {
        case number_m: {
            double key = index->data.numVal;
            auto& map = *table->numIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        case string_m: {
            std::string key(index->data.strVal);
            auto& map = *table->strIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        case bool_m: {
            bool key = index->data.boolVal;
            auto& map = *table->boolIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        case table_m: {
            avm_table* key = index->data.tableVal;
            auto& map = *table->tableIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    avm_tabledecrefcounter(key);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                } else {
                    avm_tableincrefcounter(key);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        case userfunc_m: {
            unsigned key = index->data.funcVal;
            auto& map = *table->userfuncIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        case libfunc_m: {
            std::string key(index->data.libfuncVal);
            auto& map = *table->libfuncIndexed;
            if (content->type == nil_m) {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                    map.erase(it);
                    table->total--;
                }
            } else {
                auto it = map.find(key);
                if (it != map.end()) {
                    avm_memcellclear(&it->second);
                }
                avm_memcell_copy(&map[key], content);
                if (it == map.end()) {
                    table->total++;
                }
            }
            break;
        }
        default:
            std::cerr << "Error: Invalid key type in avm_tablesetelem."
                      << std::endl;
            break;
    }
}



#ifndef AVM_TABLE_H
#define AVM_TABLE_H
#include <unordered_map>

#include "memcell_struct.h"

#define AVM_TABLE_HASHSIZE 211


typedef struct avm_table_bucket {
    avm_memcell key;
    avm_memcell value;  // Content of the element
    struct avm_table_bucket* next;
    ~avm_table_bucket();  // declare destructor
} avm_table_bucket;

typedef struct avm_table {
    unsigned refCounter;
    unsigned total;  // Total number of elements in the table
    std::unordered_map<double, avm_memcell>* numIndexed;
    std::unordered_map<std::string, avm_memcell>* strIndexed;
    std::unordered_map<bool, avm_memcell>* boolIndexed;
    std::unordered_map<avm_table*, avm_memcell, std::hash<avm_table*>>*
        tableIndexed;
    std::unordered_map<unsigned, avm_memcell>* userfuncIndexed;
    std::unordered_map<std::string, avm_memcell>* libfuncIndexed;
    avm_table();
    ~avm_table();
    unsigned get_avm_tableElement_count();
} avm_table;

void avm_memcell_copy(avm_memcell* dest, const avm_memcell* src);

avm_table* avm_tablenew(void);

void avm_tabledestroy(avm_table* t);

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index);

void avm_tablesetelem(avm_table* table, avm_memcell* index,
                      avm_memcell* content);


void avm_tableincrefcounter(avm_table* t);

void avm_tabledecrefcounter(avm_table* t);

void avm_tablebucketsinit(avm_table_bucket** buckets);
#endif  // AVM_TABLE_H
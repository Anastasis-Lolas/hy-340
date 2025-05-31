#include "avm_table.h"
#include "memcell_struct.h"
#include "../t-codeLib/t-code.h"
#include <iostream>
#include <cstring> // For memset



void avm_tablebucketsinit(avm_table_bucket** buckets) {
    for (unsigned i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        buckets[i] = (avm_table_bucket*) 0 ;
    }
}

avm_table* avm_tablenew(void) {
    avm_table* t = new avm_table;
    AVM_WIPEOUT(*t);
    if (!t) {
        std::cerr << "Error: Memory allocation failed for avm_table." << std::endl;
        return nullptr; // Handle memory allocation failure
    }
    t->refCounter = 0;
    t->total = 0;
    avm_tablebucketsinit(t->strIndexed);
    avm_tablebucketsinit(t->numIndexed);
    return t;
}

void avm_tablebucketsdestroy(avm_table_bucket** buckets) {
    if (!buckets) return;
    for (unsigned i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        avm_table_bucket* bucket = buckets[i];
        while (bucket) {
            avm_table_bucket* next = bucket->next;
            delete bucket;
            bucket = next;
        }
        buckets[i] = nullptr; // Set to nullptr after deletion
    }
}


void avm_tabledestroy(avm_table* t) {
    if (!t) return;
    avm_tablebucketsdestroy(t->strIndexed);
    avm_tablebucketsdestroy(t->numIndexed);
    delete t;
}




void avm_tableincrefcounter(avm_table* t) {
    if (t) {
        t->refCounter++;
    } else {
        std::cerr << "Error: Attempt to increment refCounter of a null table." << std::endl;
    }
}
void avm_tabledecrefcounter(avm_table* t) {
    assert(t->refCounter > 0); // Ensure refCounter is not decremented below zero
        if (!--t->refCounter) {
            avm_tabledestroy(t);
        }
    } 


avm_memcell*    avm_tablegetelem (avm_table* table, avm_memcell* index){

}

void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content) {
    if (!table || !index || !content) {
        std::cerr << "Error: Null pointer passed to avm_tablesetelem." << std::endl;
        return;
    }

}



#ifndef AVM_TABLE_H
#define AVM_TABLE_H
#include "memcell_struct.h"


#define AVM_TABLE_HASHSIZE 211

typedef struct avm_table {
    unsigned refCounter; 
    unsigned total;      // Total number of elements in the table
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE]; 
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE]; 
} avm_table;


typedef struct avm_table_bucket {
    avm_memcell          key;        
    avm_memcell          value;    // Content of the element
    struct avm_table_bucket* next; 

    ~avm_table_bucket(); // declare destructor
} avm_table_bucket;

avm_table*      avm_tablenew (void);

void            avm_tabledestroy (avm_table* t);

avm_memcell*    avm_tablegetelem (avm_table* table, avm_memcell* index);

void            avm_tablesetelem ( avm_table*  table, avm_memcell* index,avm_memcell* content);


void            avm_tableincrefcounter (avm_table* t);

void            avm_tabledecrefcounter (avm_table* t);

void            avm_tablebucketsinit (avm_table_bucket** buckets);
#endif // AVM_TABLE_H
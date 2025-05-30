#ifndef AVM_TABLE_H
#define AVM_TABLE_H
#include "memcell_struct.h"

avm_table* avm_tablenew(void);
void avm_tabledestroy(avm_table* t);

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index);

void avm_tablesetelem(avm_table* table, avm_memcell* index,
                      avm_memcell* content);


#endif  // AVM_TABLE_H
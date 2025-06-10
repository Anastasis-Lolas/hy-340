#ifndef SYMTABLE_HEADER
#define SYMTABLE_HEADER
#include <string>
#include <vector>

/*Struct for symbol Table*/
typedef struct SymTable *SymTable_T;

typedef struct hashNode {
    std::string key;
    void *value;
    struct hashNode *next;
} *hash_t;

/*
Hash Table struct
size_t size is the number of buckets
hash_t *buckets, an array of pointers
*/
struct SymTable {
    size_t size;
    std::vector<hashNode *> buckets;
};


SymTable_T SymTable_new(void);

/* Frees the memory used by the symbol table */
void SymTable_free(SymTable_T oSymTable);

/* Returns the number of the bindings */
unsigned int SymTable_getLength(SymTable_T oSymTable);

/* Adds a new bind to the symbol table */
int SymTable_put(SymTable_T oSymTable, const std::string &pcKey, void *pvValue);

/* Removes a binding from the symbol table */
int SymTable_remove(SymTable_T oSymTable, const std::string &pcKey);

/* Checks if the symbol table contains a binding*/
int SymTable_contains(SymTable_T oSymTable, const std::string &pcKey);

/* Returns the value corresponding to pckey */
void *SymTable_get(SymTable_T oSymTable, const std::string &pcKey);

/* Applies a function to each binding in the symbol table */
void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const std::string &pcKey, void *pvValue,
                                  void *pvExtra),
                  void *pvExtra);

void *SymTable_lookup(SymTable_T oSymTable, const std::string &pcKey);

void *SymTable_general_lookup(SymTable_T oSymTable, const std::string &pcKey);

void SymTable_print(SymTable_T oSymTable);
#endif

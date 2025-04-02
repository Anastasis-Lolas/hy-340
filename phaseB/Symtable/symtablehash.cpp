#include <cassert>
#include <iostream>
// #include <string>
#include <vector>

#include "TableEntry/SymbolTableEntry.h"
#include "symtable.h"


#define HASH_MULTIPLIER 65599
#define LOWER_BOUND 509
#define UPPER_BOUND 65521


/*Creates New node for the hash table*/
hash_t New_node(void) {
    hash_t ptr = new hashNode;
    if (ptr == NULL) {
        printf("error-out of memory\n");
        exit(-1);
    }
    ptr->key = "";
    ptr->next = NULL;
    ptr->value = NULL;
    return ptr;
}

/*A Static function, returns a hash code for pcKey. */
static unsigned int SymTable_hash(const std::string &pcKey);

/*A Static function that increases the number of buckets*/
static int Resize(SymTable_T oSymTable);

SymTable_T SymTable_new(void) {
    SymTable_T HashTable = new SymTable;
    if (HashTable == NULL) {
        printf("error-out of memory\n");
        exit(-1);
    }
    /*Initialize the hash table*/
    HashTable->size = LOWER_BOUND;
    HashTable->buckets.resize(LOWER_BOUND, nullptr);
    return HashTable;
}

/* Frees the memory used by the symbol table */
void SymTable_free(SymTable_T oSymTable) {
    if (oSymTable == NULL) return;

    for (auto &head : oSymTable->buckets) {
        while (head) {
            hash_t next = head->next;
            delete head;
            head = next;
        }
    }
    delete oSymTable;
}

/* Return a hash code for pcKey. */

static unsigned int SymTable_hash(const std::string &pcKey) {
    unsigned int uiHash = 0U;
    for (char ch : pcKey) {
        uiHash = uiHash * HASH_MULTIPLIER + ch;
    }

    return uiHash;
}
/* Returns the number of the nodes */
unsigned int SymTable_getLength(SymTable_T oSymTable) {
    size_t count = 0;
    assert(oSymTable);
    for (const auto &head : oSymTable->buckets) {
        for (hash_t node = head; node; node = node->next) {
            count++;
        }
    }
    return count;
}
/* Adds a new node to the hash table */
int SymTable_put(SymTable_T oSymTable, const std::string &pcKey,
                 void *pvValue) {
    size_t index;
    hash_t access;
    assert(oSymTable);

    if (SymTable_getLength(oSymTable) == oSymTable->size) {
        if (Resize(oSymTable) == 0) {
            return 0;
        }
    }

    index = SymTable_hash(pcKey) % (oSymTable->size);
    access = New_node();
    /*+1 for \0, the end of string*/
    access->key = pcKey;
    access->value = pvValue;
    access->next = oSymTable->buckets[index];
    oSymTable->buckets[index] = access;

    return 1;
}
/* Removes binding from the symbol table */
int SymTable_remove(SymTable_T oSymTable, const std::string &pcKey) {
    size_t index;
    hash_t previous = NULL, access;
    assert(oSymTable);

    index = SymTable_hash(pcKey) % (oSymTable->size);
    access = oSymTable->buckets[index];

    while (access) {
        if (access->key == pcKey) {
            if (previous) {
                previous->next = access->next;
            } else {
                oSymTable->buckets[index] = access->next;
            }
            delete access;
            return 1;
        }
        previous = access;
        access = access->next;
    }
    return 0;
}
/* Checks if the symbol table contains a binding*/
int SymTable_contains(SymTable_T oSymTable, const std::string &pcKey) {
    size_t index;
    hash_t access;
    assert(oSymTable);

    index = SymTable_hash(pcKey) % (oSymTable->size);
    access = oSymTable->buckets[index];

    for (hash_t node = oSymTable->buckets[index]; node; node = node->next) {
        if (node->key == pcKey) return 1;
    }
    return 0;
}

/* Returns the value corresponding to pckey */
void *SymTable_get(SymTable_T oSymTable, const std::string &pcKey) {
    size_t index;
    assert(oSymTable);
    index = SymTable_hash(pcKey) % (oSymTable->size);
    for (hash_t access = oSymTable->buckets[index]; access;
         access = access->next) {
        if (access->key == pcKey) return access->value;
    }
    return NULL;
}
/* Applies a function to each binding in the symbol table */
void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const std::string &pcKey, void *pvValue,
                                  void *pvExtra),
                  void *pvExtra) {
    size_t index;
    assert(pfApply);
    assert(oSymTable);
    for (auto &head : oSymTable->buckets) {
        for (hash_t access = head; access; access = access->next) {
            pfApply(access->key, access->value, pvExtra);
        }
    }
    return;
}
/*----------------BONUS----------------------------------*/

/*A Static function that increases the number of buckets*/
/*Returns 0 if the resize failed*/
static int Resize(SymTable_T oSymTable) {
    /* 509, 1021, 2053, 4093, 8191, 16381, 32771, and 65521. */
    size_t newSize, buckIndex;
    hash_t *Buckets;

    if (oSymTable == NULL) {
        return 0;
    }
    newSize = oSymTable->size;
    if (newSize == 509) {
        newSize = 1021;
    } else if (newSize == 1021) {
        newSize = 2053;
    } else if (newSize == 2053) {
        newSize = 4093;
    } else if (newSize == 4093) {
        newSize = 8191;
    } else if (newSize == 8191) {
        newSize = 16381;
    } else if (newSize == 16381) {
        newSize = 32771;
    } else if (newSize == 32771) {
        newSize = 65521;
    } else if (newSize == 65521) {
        /*Max size, size cannot be increased*/
        return 0;
    }

    std::vector<hash_t> newBuckets(newSize, nullptr);
    for (auto &acceess : oSymTable->buckets) {
        while (acceess) {
            buckIndex = SymTable_hash(acceess->key) % newSize;
            hash_t next = acceess->next;
            acceess->next = newBuckets[buckIndex];
            newBuckets[buckIndex] = acceess;
            acceess = next;
        }
    }
    // oSymTable->buckets = Buckets;
    // free(oSymTable->buckets);
    oSymTable->buckets = move(newBuckets);
    oSymTable->size = newSize;

    return 1;
}


void *SymTable_lookup(SymTable_T oSymTable, const std::string &pcKey) {
    // hashing to get the index of the table
    int index = SymTable_hash(pcKey) % (oSymTable->size);

    hash_t entry = oSymTable->buckets[index];
    SymbolTableEntry_T symbolEntry;

    while (entry) {
        symbolEntry = static_cast<SymbolTableEntry_T>(entry->value);

        if (entry->key == pcKey && symbolEntry && symbolEntry->isActive) {
            return entry->value;
        }
        entry = entry->next;
    }
    return nullptr;
}

void *SymTable_general_lookup(SymTable_T oSymTable, const std::string &pcKey) {
    // hashing to get the index of the table
    int index = SymTable_hash(pcKey) % (oSymTable->size);

    hash_t entry = oSymTable->buckets[index];
    SymbolTableEntry_T symbolEntry;

    while (entry) {
        symbolEntry = static_cast<SymbolTableEntry_T>(entry->value);

        if (entry->key == pcKey && symbolEntry) {
            return entry->value;
        }
        entry = entry->next;
    }
    return nullptr;
}


void SymTable_print(SymTable_T oSymTable) {
    if (!oSymTable) {
        printf("Symbol Table is NULL\n");
        return;
    }

    printf("========================================================\n");
    printf("                     SYMBOL TABLE\n");
    printf("========================================================\n");

    for (size_t i = 0; i < oSymTable->size; i++) {
        hash_t entry = oSymTable->buckets[i];
        if (entry) {
            printf("Bucket[%zu]: ", i);
        }
        while (entry) {
            SymbolTableEntry_T symbolEntry =
                static_cast<SymbolTableEntry_T>(entry->value);

            if (symbolEntry) {
                printf(" Name: %s | Scope: %u | Line: %u | Type: ",
                       (symbolEntry->type == USERFUNC ||
                        symbolEntry->type == LIBFUNC)
                           ? symbolEntry->value.funcVal->name.c_str()
                           : symbolEntry->value.varVal->name.c_str(),
                       (symbolEntry->type == USERFUNC ||
                        symbolEntry->type == LIBFUNC)
                           ? symbolEntry->value.funcVal->scope
                           : symbolEntry->value.varVal->scope,
                       (symbolEntry->type == USERFUNC ||
                        symbolEntry->type == LIBFUNC)
                           ? symbolEntry->value.funcVal->line
                           : symbolEntry->value.varVal->line);

                switch (symbolEntry->type) {
                    case GLOBAL:
                        printf("GLOBAL");
                        break;
                    case LLOCAL:
                        printf("LLOCAL");
                        break;
                    case FORMAL:
                        printf("FORMAL");
                        break;
                    case USERFUNC:
                        printf("USERFUNC");
                        break;
                    case LIBFUNC:
                        printf("LIBFUNC");
                        break;
                    default:
                        printf("UNKNOWN");
                        break;
                }

                printf(" | Active: %s |", symbolEntry->isActive ? "Yes" : "No");


                if (symbolEntry->type == USERFUNC ||
                    symbolEntry->type == LIBFUNC) {
                    printf("      Args: [");
                    for (size_t j = 0;
                         j < symbolEntry->value.funcVal->args.size(); j++) {
                        printf("arg%zu", j);
                        if (j < symbolEntry->value.funcVal->args.size() - 1)
                            printf(", ");
                    }
                    printf("]\n");
                } else
                    printf("\n");
            } else {
                printf("Bucket[%zu]: INVALID ENTRY\n", i);
            }

            entry = entry->next;
            if (entry) {
                printf("\t\t");
            }
        }
    }

    printf("========================================================\n");
}

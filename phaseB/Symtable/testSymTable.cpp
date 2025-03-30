#include <iostream>

// #include "symtable.h"
#include "SymTable.h"


using namespace std;

void printKeyValue(const string& key, void* value, void* extra) {
    cout << "Key: " << key << ", Value: " << *static_cast<int*>(value) << endl;
}

int main() {
    SymTable* table = SymTable_new();

    int val1 = 10, val2 = 20, val3 = 30;
    SymTable_put(table, "apple", &val1);
    SymTable_put(table, "banana", &val2);
    SymTable_put(table, "cherry", &val3);

    cout << "Table contains 'apple'? "
         << (SymTable_contains(table, "apple") ? "Yes" : "No") << endl;
    cout << "Table contains 'grape'? "
         << (SymTable_contains(table, "grape") ? "Yes" : "No") << endl;

    int* retrievedValue = static_cast<int*>(SymTable_get(table, "banana"));
    if (retrievedValue) {
        cout << "Value associated with 'banana': " << *retrievedValue << endl;
    } else {
        cout << "'banana' not found in table." << endl;
    }

    cout << "Number of elements in table: " << SymTable_getLength(table)
         << endl;

    SymTable_map(table, printKeyValue, nullptr);

    SymTable_remove(table, "apple");
    cout << "After removing 'apple', table contains 'apple'? "
         << (SymTable_contains(table, "apple") ? "Yes" : "No") << endl;

    cout << "Resizing table... " << (Resize(table) ? "Success" : "Failed")
         << endl;

    SymTable_free(table);
    return 0;
}

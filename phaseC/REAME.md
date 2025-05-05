# Alpha Language Compiler – Phase 2 

## Authors  
Anastasios Lolas csd5197
Alexandros Manos csd5136
Paraskevi Mourelatou csd5149

# Commands for makefile:
(1) make
(2) ./scanner < tests/inputFile

# Computer used : kerasi 

# Description


# Project Overview
The parser:
- Tables initialized once at the start. 
- Tracks and prints the grammar rules.
- Constructs and updates a symbol table as identifiers appear.
- Prints the Final symbol table with each scope. 

# symtable.h and symtablehash.cpp  
Implements a hash table for storing symbols (variables, functions).  
- Uses buckets and each bucket is a linked list of hashNode entries. 
- Fast insert and lookup by identifier name. To be precise, the SymTable_lookup function searches
  for a symbol in the hash table and returns it only if it is currently active. 
- Stores symbol (type, scope, line).

# scopelist.cpp  
Tracks symbols by scope using a vector of lists.  
- Each index = one scope level.  
- Allows scope-based lookups and visibility control.  
- Initializes library functions at global scope.

# expr_handler.cpp  
The main Link between parser and symbol managment functions
- it tracks scope, line, and function numbering.  
- adds new variables/functions with correct type and scope.  
- Handles local and :: keywords and also ++ -- and assign checks (functions shouldn't use them)
- it prevents illegal shadowings and redefinitions .  
- Provides debugging prints and the main printFullSymtable function.




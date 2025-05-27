# Alpha Language Compiler – Phase 3 

## Authors  
Anastasios Lolas csd5197
Alexandros Manos csd5136
Paraskevi Mourelatou csd5149

# Commands for makefile:
(1) make
(2) ./scanner < tests/inputFile

# Computer used : kerasi 

# Description

### What We Support:

- arithmetic expressions
- boolean expressions (AND, OR, NOT)
- if-else and loops, 
- Function calls
- Method calls 
- Expression evaluation with short-circuiting for booleans
- Array implementation

# Project code Overview


# quads.cpp and quads.h  
Handles generation and storage of intermediate code (quads).
- defines the structure of quads and  opcodes
- generates quads with emit function.
- provides functions to patch labels (patchlabel), manage lists for control flow (patchlist, mergelist) and loops.


# expression.h
- defines the expr structure.
- Stores expression data, including symbol table entries, constants,
   and True/False lists for boolean expressions.
- Supports call structures (call_t) 



# expr_handler.cpp and expr_handler.h
The main Link between parser and symbol managment functions and also the core for expression management 

- it inncludes functions to create and change expressions (e.g., newexpr, emit_assign_exp) 
- Support function calls, method calls and anonymous functions
- type checking and error handling for operations.  
- we connect expressions to symbols in the symbol table using symEntr_to_expr();
- Boolean expressions (like in AND, OR, and NOT) we use short-circuit logic, with expr* to_boolexpr(expr* e);




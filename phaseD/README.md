# Alpha Language Compiler – Phase 4,5

## Authors  
- Anastasios Lolas (csd5197)  
- Alexandros Manos (csd5136)  
- Paraskevi Mourelatou (csd5149)  

## Build & Execution Instructions

1. Compile the project:  
   make

2. Run the scanner on an input file:  
   ./scanner < tests/inputFile

3. Navigate to the AVM (Alpha Virtual Machine) directory:  
   cd AVM/

4. Execute the Alpha Virtual Machine:  
   ./avm

Development Machine: kerasi

---

## Project Overview

This final phase of the Alpha Language Compiler project (HY-340) introduces the binary format for intermediate code and the implementation of a Virtual Machine (VM) capable of executing Alpha language programs. The key additions in this phase are:

- Binary file I/O support for the intermediate representation.
- A fully functioning Virtual Machine for the Alpha language, capable of decoding and executing binary instruction sets.

---

## Supported Language Features

The current implementation supports the following core features of the Alpha language:

- Conditional execution via if statements  
- Loop constructs: for and while  
- Variable assignment and expression evaluation  
- Complete suite of arithmetic operations  
- Invocation of both user-defined and standard library functions  
- Creation, manipulation, and deletion of table structures (similar to associative arrays)

---

## AVM Directory Structure

The AVM/ directory contains the complete implementation of the Alpha Virtual Machine. Key components include:

- Avm_main.cpp: Entry point and orchestrator of the VM runtime  
- Modular subdirectories organizing:
  - Data Structures: Definitions of runtime entities such as memory cells, tables, and environments  
  - AVM Core Functions: Handlers for arithmetic operations, library functions, and user function calls  
  - Execution Engine: Logic for decoding instructions, managing the stack, and executing bytecode  


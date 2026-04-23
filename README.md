# HY-340 — Alpha Language Compiler & Virtual Machine (AVM)

Course project for **HY-340** implementing a compiler toolchain for the **Alpha** language, including:
- A **scanner/lexer** (Flex / Lex)
- Parsing components (Yacc)
- Compiler phases that produce intermediate / binary representations
- An **Alpha Virtual Machine (AVM)** capable of executing Alpha bytecode

> Tip: There is also a more detailed phase README at `phaseD/README.md`.

---

## Table of Contents
- [Project Structure](#project-structure)
- [Build](#build)
- [Run](#run)
- [Run Tests](#run-tests)
- [Notes / Common Pitfalls](#notes--common-pitfalls)
- [Supported Language Features](#supported-language-features)
- [Authors](#authors)

---

## Project Structure

High-level components you will find in this repository:

- `scanner.l` (or similar): Flex lexer rules (generates `scanner.cpp`)
- Parser files (`.y`): Yacc grammar & parser generation
- `AVM/`: Alpha Virtual Machine implementation (executes produced bytecode)
- `tests/`: input/output test assets and test runner integration
- `phaseD/`: README + deliverables for later phases (VM + binary format)

---

## Build

### Build using Makefile (recommended)
```bash
make
```

### Manual build (lexer-only / quick check)
```bash
flex -o scanner.cpp scanner.l
g++ scanner.cpp -o scanner
```

---

## Run

Run the scanner/compiler using redirected input:

```bash
./scanner < tests/inputFile
```

(Or with your own file)
```bash
./scanner < input.txt
```

---

## Run Tests

A simple test helper is available through `make test`.

```bash
make test FILE=testname
```

Example (operators test):
```bash
make test FILE=operator1
```

### What the test script does
For a given `FILE=xx`, it:
1. Runs the program with input from `xx.in`
2. Captures the produced output
3. Compares it against `xx.out` (expected output)

---

## Notes / Common Pitfalls

- **Missing newline at end of output files can cause false test failures.**
  If you transferred files and tests suddenly fail, ensure the expected `.out` files end with a newline.

---

## Supported Language Features

Depending on the phase you are running, the implementation supports core Alpha features such as:

- Conditional execution (`if`)
- Loops (`for`, `while`)
- Variable assignment and expression evaluation
- Arithmetic operations
- User-defined and standard library function calls
- Table structures (associative arrays): creation, manipulation, deletion

---

## AVM (Alpha Virtual Machine)

To run the VM component (later phases):

```bash
cd AVM/
./avm
```

---

## Authors

- Anastasios Lolas (csd5197)  
- Alexandros Manos (csd5136)  
- Paraskevi Mourelatou (csd5149)  

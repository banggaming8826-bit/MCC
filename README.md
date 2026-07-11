# MCC

**MCC** is an experimental programming language and compiler written in modern C++.

This project is being developed from scratch, including:

- Lexer
- Recursive Descent Parser
- AST
- x86-64 Assembly Code Generator

The goal of MCC is to explore compiler implementation and language design while creating a practical programming language.

>> [###] **Status:** Work In Progress 🚧

---

## Current Features

- [x] Variable declaration
- [x] Arithmetic expressions
- [x] Assignment
- [x] `if / elif / else`
- [x] `while`
- [x] `for`
- [x] AST generation
- [x] x86/x86-64 Assembly generation (partial)
- [x] Function parser

## Planned Features

- [ ] Function code generation
- [ ] `return <value...>`
- [ ] Function call
- [ ] Pointer
- [ ] Reference
- [ ] Array
- [ ] `import "path/to/lib..."`
- [ ] Standard library (`basicio.mc`, ...)
- [ ] Optimizer

---

## Example

```mcc
char a = 255;
short b = 32767;
int c = 30 * 12;
float x = 3.14;
double y = 2.71

if (a == 30 * 12) {
    a = 20;
}
```

---

## Project Structure

```
lexer.h        - Lexer
parser.h       - Recursive Descent Parser
ast.h          - Abstract Syntax Tree
buildasm.h     - Assembly Code Generator
general.h      - Shared definitions
mcc.cpp        - Entry point
```

---

## Target

- Language: **MCC**
- Backend: **x86/x86-64 Assembly (NASM)**

---
^^ thanks

# MCC

**MCC** is an experimental programming language and compiler written in modern C++.

This project is being developed from scratch, including:

- Lexer
- Recursive Descent Parser
- AST
- x86-64 Assembly Code Generator

The goal of MCC is to explore compiler implementation and language design while creating a practical programming language.

>> [#] **Status:** Work In Progress 🚧

---

## Current Features

- [x] Variable declaration
- [x] Arithmetic expressions
- [x] Assignment
- [x] `if / elif / else`
- [x] `while`
- [x] `for`
- [x] AST generation
- [x] x86-64 Assembly generation (partial)
- [x] Function parser

## Planned Features

- [ ] Function code generation
- [ ] `return`
- [ ] Function call
- [ ] Pointer
- [ ] Reference
- [ ] Array
- [ ] `import`
- [ ] Standard library (`basicio.mc`)
- [ ] Optimizer

---

## Example

```mcc
int a = 10;

if (a > 5)
{
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

# Lexical Analyzer in C 

A robust **Lexical Analyzer** built in C for a **subset of the C language**, capable of tokenizing source code and performing basic syntax validation. It detects tokens like keywords, identifiers, literals (decimal, float, string, char), arrays, and also handles hexadecimal, binary, and octal formats.

---

## 🚀 Features

- ✅ Supports a **subset of C language**
- ✅ Identifies:
  - **Keywords** (e.g., `int`, `return`)
  - **Identifiers** (e.g., `my_var`)
  - **Literals**:  
    - **Decimal, Float, Char, String**  
    - **Hexadecimal** (e.g., `0xabcd`)  
    - **Binary** (e.g., `0b10101`)  
    - **Octal** (e.g., `076`)
- ⚠️ Warns if **braces `{}` are unmatched**
- ❌ **Errors and stops** parsing for:
  - Malformed literals like `08` (invalid octal), `0b102` (invalid binary)
  - Unmatched or missing quotes (`"`, `'`)
  - Invalid variable names or corrupted token patterns

---

## 🛠 Technologies Used

- **Language**: C  
- **Concepts**:
  - File Handling
  - Lexical Tokenization
  - Regular Pattern Matching
  - Error & Warning Reporting
  - Custom Syntax Rules

---

## 📦 How to Compile & Run

1. Compile the program: `gcc lexer.h lexer.h main.c`.
2. Then run program: `./a.out <any c file>`.
3. Example: Compile code and give `./a.out test.c`.
   

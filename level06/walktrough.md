<h1 align="center"> LEVEL 6 </h1>

## 🔍 Analysis of Decompiled [level6](./pseudo.c)

**Purpose:** Complete, step‑by‑step explanation of the vulnerability in the provided `auth()` / `main()` code, how it can be exploited, and a reproducible exploitation procedure. This file focuses on *the logic vulnerability* (not a memory corruption bug) and how to compute a valid serial to get a shell.

---

## 1. TL;DR

* The program implements a **custom authentication hash**: it computes a numeric value from the supplied login string and compares it to the supplied serial.
* There is **no buffer overflow** or direct memory corruption in `auth()` — the flaw is *design/logic*: the serial is fully determined by the login so it can be generated offline.
* The program uses `ptrace(PTRACE_TRACEME)` to detect debuggers and rejects traced runs; run the program normally (or bypass the check) during exploitation.
* Exploitation is trivial: **reimplement the algorithm** (script provided) and compute the correct serial, then supply the login + computed serial to get `system("/bin/sh")`.

---

## 2. Relevant code (decompiled highlights)

```c
// auth(char *param_1, uint param_2)
size_t sVar1 = strcspn(param_1, "\n");
param_1[sVar1] = '\0';                 // strip trailing newline
sVar1 = strnlen(param_1, 0x20);
if ((int)sVar1 < 6) return 1;          // require length >= 6

long lVar3 = ptrace(PTRACE_TRACEME);
if (lVar3 == -1) {                     // anti-debug: if traced -> fail
  puts("!! TAMPERING DETECTED !!");
  return 1;
}

uint local_14 = ((int)param_1[3] ^ 0x1337U) + 0x5eeded;
for (int i = 0; i < (int)sVar1; i++) {
  if (param_1[i] < ' ') return 1;      // no control chars allowed
  local_14 = local_14 + ((int)param_1[i] ^ local_14) % 0x539;
}

if (param_2 == local_14) return 0;     // success -> 0, caller runs system("/bin/sh")
else return 1;
```

In `main()`: reads login via `fgets`, reads serial (scanf), calls `auth(login, serial)`; if return == 0 => shell.

---

## 3. Detailed vulnerability explanation (line‑by‑line)

1. **Input normalization**

   * `strcspn` + `param_1[sVar1] = '\0';` removes the newline that `fgets` keeps. This is standard and correct.

2. **Length check**

   * `sVar1 = strnlen(param_1, 0x20); if ((int)sVar1 < 6) return 1;` — enforces minimum login length of 6. Short inputs are rejected.

3. **Anti-debugging**

   * `ptrace(PTRACE_TRACEME)` returns -1 when the process is being traced by a debugger. If traced, the program prints the tampering message and fails authentication. This **prevents interactive debugging** unless you bypass the check (see bypass section).

4. **Seed computation**

   * `local_14 = ((int)param_1[3] ^ 0x1337U) + 0x5eeded;`
   * Only the 4th character (`param_1[3]`) is used to seed the checksum, combined with constants. This makes the resulting serial *depend strongly* on the login.

5. **Per‑character hash update**

   * For each character `c` in the login:

     * If `c < 0x20` (control char) return failure.
     * `local_14 = local_14 + ((int)c ^ local_14) % 0x539;`
   * This updates the accumulator using XOR with the current value, then reduces by modulo `0x539` (1337 decimal). The algorithm is deterministic and reversible in the sense that you can compute the final result given the login.

6. **Comparison**

   * `if (param_2 == local_14) success else fail` — attacker-supplied serial is compared to computed value. Because the serial is computed deterministically from login, **we can compute the correct serial offline** and feed it to the program.

7. **Result**

   * If the function returns 0, `main()` runs `system("/bin/sh")`, providing a shell. Therefore the only barrier is computing the correct serial.

---

## Exploitation strategy (high level)

1. **Choose a printable login of length >= 6.**
2. **Reimplement the hash in a script** (Python in our case) exactly as in the binary.
3. **Compute the serial value** for chosen login.
4. **Run the target program normally (not under a debugger)** and provide the login and computed serial.
5. Program authenticates and spawns a shell.

This is not memory exploit - it is logic bypass via reverse engineering.

---

## Reproducible exploit steps (commands)

1. Save the Python serial generator (example below) as `serial_gen.py` and make executable.

```python
#!/usr/bin/env python3

def generate_serial(login: str) -> int:
    login = login.split('\n')[0]          # strip newline
    if len(login) < 6:
        raise ValueError("Login must be at least 6 characters")
    serial = (ord(login[3]) ^ 0x1337) + 0x5eeded
    for c in login:
        if ord(c) < 0x20:
            raise ValueError("Invalid character in login")
        serial += (ord(c) ^ serial) % 0x539
    return serial

if __name__ == '__main__':
    import sys
    login = input('Login: ')
    print(generate_serial(login))
```

2. Generate a serial for a chosen login:

```bash
$ python3 serial_gen.py
Login: checkme
12345678    # (example output)
```

3. Run the target and provide inputs:

```bash
$ ./level06
-> Enter Login: checkme
-> Enter Serial: 12345678
Authenticated!
$ whoami
level07
$ cd ../level07
$ cat .pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

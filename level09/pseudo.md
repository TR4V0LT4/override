Source Code Analysis:

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

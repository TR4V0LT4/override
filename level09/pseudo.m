Source Code Analysis:
main
 └─ handle_msg
     ├─ set_username
     └─ set_msg

Main Function

```c
cundefined8 main(void) {
    puts("--------------------------------------------\n"
         "| ~Welcome to l33t-m$n ~ v1337             |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}
```

Handle Message Function
```c
void handle_msg(void) {
    char buffer[140];   // Main buffer
    int size = 140;     // Copy size (0x8c)

    set_username(buffer);
    set_msg(buffer);

    puts(">: Msg sent!");
}
```

Key Points:
- buffer is 140 bytes
- size is stored right after the buffer on the stack
- Both set_username() and set_msg() receive a pointer to buffer


Set Username Function

```c
void set_username(char *buf) {
    char username[140];

    puts(">: Enter your username");
    fgets(username, 128, stdin);

    // Copy username into buf + 140
    for (int i = 0; i < 41 && username[i]; i++) {
        buf[140 + i] = username[i];
    }

    printf(">: Welcome, %s", buf + 140);
}
```
Key Points:

- Reads up to 0x80 (128) bytes from stdin
- Copies up to 0x29 (41) bytes to param_1 + 140
- Vulnerability #1: Loop copies 41 bytes starting at offset 140
- Can overflow from param_1 + 140 to param_1 + 140 + 41 = param_1 + 0xb5 
- This allows overwriting the size variable at param_1 + 0xb4!


Set Message Function
```c
void set_msg(char *buf) {
    char msg[1024];

    puts(">: Msg @Unix-Dude");
    fgets(msg, 1024, stdin);

    strncpy(buf, msg, *(int *)(buf + 180)); // size variable
}

```
Key Points:

- Reads up to 0x400 (1024) bytes into local buffer
- Calls strncpy(buf, msg, SIZE)
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

int main(int argc, char **argv) {
    srand(time(NULL));
    
    printf("Password: ");
    unsigned int input;
    scanf("%u", &input);
    
    test(input, 0x1337d00d);  // 0x1337d00d = 322424845
    return 0;
}

int test(int a, int b) {
    int diff = b - a; 
    
    switch (diff) {
        case 1: case 2: case 3: case 4: case 5:
        case 6: case 7: case 8: case 9:
        case 16: case 17: case 18: case 19:
        case 20: case 21:
            return decrypt(diff);  // Use difference as decryption key!
    }
    
    // If difference not in valid range, use random key
    return decrypt(rand());
}

int decrypt(int key) {
    char s[17];
    strncpy(s, "Q}|u`sfg~sf{}|a3", 17);  // Encrypted password
    
    // XOR decrypt each character
    for (int i = 0; i < 16; i++)
        s[i] ^= key;
    
    if (strcmp(s, "Congratulations!") == 0) {
        system("/bin/sh");  // Shell access!
        return 0;
    } else {
        puts("\nInvalid Password");
        return -1;
    }
}
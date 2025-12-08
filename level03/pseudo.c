#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear_stdin() {
    int c;
    while (1) {
        c = getchar();
        if (c == '\n' || c == EOF)
            break;
    }
}

unsigned int get_unum() {
    unsigned int result = 0;
    fflush(stdout);
    scanf("%u", &result);
    clear_stdin();
    return result;
}

// The encrypted string inside decrypt():
// "Q}|u`sfg~sf{}|a3"  (17 chars)
int decrypt(int key) {
    char s[17];
    strncpy(s, "Q}|u`sfg~sf{}|a3", 17);

    // XOR-decrypt string
    for (int i = 0; i < 16; i++)
        s[i] ^= key;

    // Compare with correct phrase
    if (strcmp(s, "Congratulations!") == 0) {
        system("/bin/sh");
        return 0;
    } else {
        puts("\nInvalid Password");
        return -1;
    }
}

int test(int a, int b) {
    int diff = b - a;

    switch (diff) {
        case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8:
        case 9: case 16: case 17: case 18:
        case 19: case 20: case 21:
            return decrypt(diff);
    }

    // otherwise random key
    return decrypt(rand());
}

int main(int argc, char **argv) {
    srand(time(NULL));

    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password: ");

    unsigned int input;
    scanf("%u", &input);

    // Compare user input with 0x1337d00d
    test(input, 0x1337d00d);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE *local_10 = NULL;
    char local_118[112]; /* buffer for password input */
    char local_a8[48]; /* buffer for password from file */
    char local_78[100]; /* buffer for username input */
    size_t sVar2; /* for return values of fread and strcspn */
    int local_14 = 0; /* length of password read */

    /* zero buffers like the original */
    memset(local_78, 0, sizeof(local_78));
    memset(local_a8, 0, sizeof(local_a8));
    memset(local_118, 0, sizeof(local_118));

    local_10 = fopen("/home/users/level03/.pass", "r");
    if (local_10 == NULL) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }

    sVar2 = fread(local_a8, 1, 0x29, local_10); /* read 0x29 (41) bytes */
    local_14 = (int)sVar2;

    /* strip newline if present */
    sVar2 = strcspn(local_a8, "\n");
    if (sVar2 < sizeof(local_a8)) local_a8[sVar2] = '\0';

    if (local_14 != 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fclose(local_10);
        exit(1);
    }

    fclose(local_10);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    printf("--[ Username: ");
    if (fgets(local_78, (int)sizeof(local_78), stdin) == NULL) {
        /* treat as failed input */
        puts("No username input");
        exit(1);
    }
    sVar2 = strcspn(local_78, "\n");
    if (sVar2 < sizeof(local_78)) local_78[sVar2] = '\0';

    printf("--[ Password: ");
    if (fgets(local_118, (int)sizeof(local_118), stdin) == NULL) {
        puts("No password input");
        exit(1);
    }
    sVar2 = strcspn(local_118, "\n");
    if (sVar2 < sizeof(local_118)) local_118[sVar2] = '\0';

    puts("*****************************************");

    /* compare up to 0x29 bytes */
    if (strncmp(local_a8, local_118, 0x29) == 0) {
        printf("Greetings, %s!\n", local_78);
        /* spawn a shell like original */
        system("/bin/sh");
        return 0;
    }

    /* original printed the username then " does not have access!" */
    printf("%s", local_78);
    puts(" does not have access!");
    exit(1);
}

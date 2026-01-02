#include <stdio.h>
#include <string.h>

int verify_user_name(char *username)
{
    return strcmp(username, "dat_wil");
}

int verify_user_pass(char *password)
{
    return strcmp(password, "admin");
}

int main()
{
    char username[256];
    char password[64];  
    
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(username, 256, stdin);
    
    if (verify_user_name(username) != 0)
    {
        puts("nope, incorrect username...\n");
        return 1;
    }
    
    printf("Enter Password: ");
    fgets(password, 100, stdin);
    
    if (verify_user_pass(password) == 0)
    {
        puts("Access granted!\n");
        return 0;
    }
    
    puts("nope, incorrect password...\n");
    return 1;
}

int32_t verify_user_name()
{
    int32_t __saved_esi;
    bool c = &__saved_esi < 0x10;
    bool z = &__saved_esi == 0x10;
    puts("verifying username....\n");
    int32_t i = 7;
    char* esi = &a_user_name;
    char* edi = "dat_wil";
    
    // ... comparison loop ...
    while (i)
    {
        char temp1_1 = *esi;
        char temp2_1 = *edi;
        c = temp1_1 < temp2_1;
        z = temp1_1 == temp2_1;
        esi = &esi[1];
        edi = &edi[1];
        i -= 1;
        
        if (!z)
            break;
    }
    
    return (!z && !c) - c;
}

int32_t verify_user_pass(char* arg1)
{
    int32_t i = 5;
    char* esi = arg1;
    char* edi = "admin";
    bool c;
    bool z;
    
    // ... comparison loop ...
    while (i)
    {
        char temp0_1 = *esi;
        char temp1_1 = *edi;
        c = temp0_1 < temp1_1;
        z = temp0_1 == temp1_1;
        esi = &esi[1];
        edi = &edi[1];
        i -= 1;
        
        if (!z)
            break;
    }
    
    char* edx;
    edx = !z && !c;
    return edx - c;
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    void buf;
    __builtin_memset(&buf, 0, 0x40);
    int32_t var_14 = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(&a_user_name, 0x100, stdin);
    
    if (verify_user_name())
    {
        puts("nope, incorrect username...\n");
        return 1;
    }
    
    puts("Enter Password: ");
    fgets(&buf, 0x64, stdin);
    int32_t eax_2 = verify_user_pass(&buf);
    
    if (eax_2 && !eax_2)
        return 0;
    
    puts("nope, incorrect password...\n");
    return 1;
}

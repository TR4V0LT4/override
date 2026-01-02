int main(void)
{
    char buffer[128];
    pid_t pid;
    int status;
    long syscall;

    pid = fork();

    if (pid == 0) {
        // ---- CHILD ----
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);

        puts("Give me some shellcode, k");
        gets(buffer);              // VULNERABILITY (no bounds check)
        return 0;
    } else {
        // ---- PARENT ----
        while (1) {
            wait(&status);

            // Child exited normally or by signal
            if ((status & 0x7f) == 0 || WIFSIGNALED(status)) {
                puts("child is exiting...");
                return 0;
            }

            // Read syscall number from child (EAX)
            syscall = ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0);

            // 0x0b == execve syscall number (x86)
            if (syscall == 0x0b) {
                puts("no exec() for you");
                kill(pid, SIGKILL);
                return 0;
            }
        }
    }
}

int main(int argc, char **argv)
{
    FILE *log;
    FILE *input;
    int out_fd;
    char out_path[104];
    char ch;

    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        // program continues (bug, but not exploitable)
    }

    // Open log file
    log = fopen("./backups/.log", "w");
    if (!log) {
        printf("ERROR: Failed to open ./backups/.log\n");
        exit(1);
    }

    // Log start message
    log_wrapper(log, "Starting back up: ", argv[1]);

    // Open input file
    input = fopen(argv[1], "r");
    if (!input) {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    // Build output path: "./backups/" + filename
    strncpy(out_path, "./backups/", 11);
    strncat(out_path, argv[1], 99);

    // Open destination file
    out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0660);
    if (out_fd < 0) {
        printf("ERROR: Failed to open ./backups/%s\n", argv[1]);
        exit(1);
    }

    // Copy file byte by byte
    while ((ch = fgetc(input)) != EOF) {
        write(out_fd, &ch, 1);
    }

    // Log end message
    log_wrapper(log, "Finished back up ", argv[1]);

    fclose(input);
    close(out_fd);
    return 0;
}

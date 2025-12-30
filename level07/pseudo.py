store_number(data):
    print "Number: "
    number = read_unsigned_int()

    print "Index: "
    index = read_unsigned_int()

    // Weak protection
    if index % 3 == 0 OR highest_byte(number) == 0xb7:
        print "This index is reserved"
        return 1

    // VULNERABILITY: NO BOUNDS CHECK
    data[index] = number
    return 0


read_number(data):
    print "Index: "
    index = read_unsigned_int()
    print data[index]

    
main(argc, argv, envp):
    data[100] = {0}
    command[20] = {0}

    // Wipe argv and envp from memory (anti-exploit noise)
    for each arg in argv:
        memset(arg, 0, strlen(arg))
    for each env in envp:
        memset(env, 0, strlen(env))

    print welcome banner

    loop forever:
        print "Input command: "
        fgets(command, 20, stdin)
        command = strip_newline(command)

        if command == "store":
            result = store_number(data)

        else if command == "read":
            result = read_number(data)

        else if command == "quit":
            exit cleanly

        else:
            result = 1

        if result == 0:
            print "Completed command successfully"
        else:
            print "Failed to do command"

        clear(command)


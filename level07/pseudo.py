import sys
import os

def store_number(data):
    print("Number: ")
    number = read_unsigned_int()

    print("Index: ")
    index = read_unsigned_int()

    # Weak protection
    if index % 3 == 0 or highest_byte(number) == 0xb7:
        print("This index is reserved")
        return 1

    # VULNERABILITY: NO BOUNDS CHECK
    data[index] = number
    return 0


def read_number(data):
    print("Index: ")
    index = read_unsigned_int()
    print(data[index])
    return 0


def main():
    data = [0] * 100
    # command buffer is simulated by using a string and truncating to 20 chars
    command = ""

    # Wipe argv and envp from memory (anti-exploit noise)
    for i in range(len(sys.argv)):
        sys.argv[i] = ""
    for k in list(os.environ.keys()):
        os.environ[k] = ""

    print("Welcome to the number store")

    while True:
        print("Input command: ", end="")
        command = sys.stdin.readline(20)
        if not command:
            break
        command = command.rstrip("\n")[:20]

        result = 1

        if command == "store":
            result = store_number(data)
        elif command == "read":
            result = read_number(data)
        elif command == "quit":
            break
        else:
            result = 1

        if result == 0:
            print("Completed command successfully")
        else:
            print("Failed to do command")

        command = ""


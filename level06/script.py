#!/usr/bin/env python3

def generate_serial(login: str) -> int:
    # Step 1: strip newline (same as strcspn)
    login = login.split('\n')[0]

    # Step 2: length check (must be >= 6)
    if len(login) < 6:
        raise ValueError("Login must be at least 6 characters")

    # Step 3: initial value
    serial = (ord(login[3]) ^ 4919) + 6221293

    # Step 4: main hash loop
    for c in login:
        if ord(c) < 32:
            raise ValueError("Invalid character in login")
        serial += (ord(c) ^ serial) % 1337

    return serial


if __name__ == "__main__":
    login = input("Login: ")
    serial = generate_serial(login)
    print(f"Serial: {serial}")

# Decrypt the password with key 18
encrypted = "Q}|u`sfg~sf{}|a3"
decrypted = "".join(chr(ord(c) ^ 18) for c in encrypted)
print(decrypted)  # Output: "Congratulations!"
# Decrypt the password with key 18
encrypted = "Congratulations!"
decrypted = "".join(chr(ord(c) ^ 18) for c in encrypted)
print(decrypted)
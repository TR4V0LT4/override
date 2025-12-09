# #!/usr/bin/env python3
# """
# Simple XOR Encrypter
# Encrypts "Congratulations!" to the hex representation used in level03
# """

# def xor_encrypt(plaintext, key):
#     """Encrypt plaintext using XOR with given key"""
#     return "".join(chr(ord(c) ^ key) for c in plaintext)

# def plaintext_to_hex(plaintext, key):
#     """Convert plaintext to encrypted hex representation"""
#     encrypted = xor_encrypt(plaintext, key)
    
#     # Convert to hex representation
#     hex_bytes = "".join(f"\\x{ord(c):02x}" for c in encrypted)
    
#     return encrypted, hex_bytes

# # Main
# plaintext = "Congratulations!"
# key = 18

# encrypted, hex_repr = plaintext_to_hex(plaintext, key)

# print("=== XOR Encryption ===\n")
# print(f"Plaintext:  '{plaintext}'")
# print(f"Key:        {key}")
# print(f"Encrypted:  '{encrypted}'")
# print(f"Hex repr:   {hex_repr}")
# print()
# print(f"Result: {encrypted}")

hex_values = [0x756e505234376848, ...]  # From binary analysis

for v in hex_values:
    hex_str = f'{v:016x}'
    reversed_hex = hex_str[::-1]
    reversed_val = int(reversed_hex, 16)
    print(struct.pack('>Q', reversed_val).decode('latin-1', errors='ignore'))
# Convert hex to ASCII
hex_values = [
    0x756e505234376848,
    0x45414a3561733951,
    0x377a7143574e6758,
    0x354a35686e475873,
    0x48336750664b394d
]
value = ""
for val in hex_values:
    ascii_str = bytes.fromhex(hex(val)[2::]).decode('latin-1')
    print(f"{hex(val)} -> {ascii_str[::-1]}")
    value += ascii_str[::-1]

print("Final value:", value)
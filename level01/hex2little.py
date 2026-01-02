def to_little_endian(addr_str):
    clean = addr_str.replace("0x", "").rjust(8, "0")
    b = bytes.fromhex(clean)
    return ''.join(f'\\x{byte:02x}' for byte in b[::-1])

sh = "0xf7f897ec"
system = "0xf7e6aed0"

print("sh     --> ",to_little_endian(sh))
print("system --> ",to_little_endian(system))
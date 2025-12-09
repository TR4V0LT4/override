def to_little_endian(addr_str):
    # remove "0x" and pad to 8 hex digits
    clean = addr_str.replace("0x", "").rjust(8, "0")
    # convert to bytes
    b = bytes.fromhex(clean)
    # reverse for little endian
    return ''.join(f'\\x{byte:02x}' for byte in b[::-1])

# example
sh = "0xf7f897ec"
system = "0xf7e6aed0"

print("sh     --> ",to_little_endian(sh))
print("system --> ",to_little_endian(system))
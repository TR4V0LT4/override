<h1 align="center"> LEVEL 3 </h1>

## 🔍 Analysis of Decompiled [level3](./pseudo.c)

This level exploits weak cryptography and flawed validation logic. The program uses a **simple XOR cipher** to encrypt a password and a weak **difference-based validation** that can be bypassed with basic arithmetic.

### 1. **Weak XOR Encryption**

The password is encrypted using simple XOR cipher:

```
Plaintext:  "Congratulations!"
Key:        18 (single byte)
Encrypted:  "Q}|u`sfg~sf{}|a3"

Process: plaintext[i] XOR 18 = encrypted[i]
```

**Why it's weak:**
- XOR with single byte key is trivially reversible
- The encrypted string is hardcoded in the binary
- With only 256 possible keys, brute force takes milliseconds

### 2. **Flawed Validation Logic**

The program calculates: `difference = 0x1337d00d - user_input`

Then accepts the difference as the decryption key IF it's in the valid range `[1-21]` (with gaps).

**The Critical Flaw:**
- We can calculate ANY valid input that produces the correct difference (key 18)
- `0x1337d00d - 18 = 322424827` ← This is the answer!

### 3. **Known Plaintext Attack**

Since we know:
- Encrypted text: `"Q}|u`sfg~sf{}|a3"`
- Expected plaintext: `"Congratulations!"` (obvious from code)
- We can calculate the key: `Q ^ Congratulations = 18`

## Exploitation Steps

### Step 1: Understand the Encryption

```c
char encrypted = 'Q';         // 0x51
char plaintext = 'C';         // 0x43
char key = 'Q' ^ 'C';         // 0x51 ^ 0x43 = 18
```
run [verify.py](verify.py) to verify.

### Step 2: Find the Valid Input Number

The program needs: `0x1337d00d - input = 18`

**Calculation:**
```
0x1337d00d = 322424845 (decimal)
322424845 - input = 18
input = 322424845 - 18
input = 322424827
```


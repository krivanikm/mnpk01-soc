---
title: ALU
---

# ALU – arithmetic logic unit

File: `rtl/alu/alu.sv`

A combinational 8-bit ALU. It has two operands `a` and `b`, a 4-bit operation code `op`
and it sets three flags.

## Operations

| Code | Name | Result | Carry (C) |
|---|---|---|---|
| `0000` | ADD | `a + b` | carry out of bit 7 |
| `0001` | SUB | `a - b` | borrow (`a < b`) |
| `0010` | INC | `a + 1` | carry out of bit 7 |
| `0011` | DEC | `a - 1` | borrow (`a < 1`) |
| `0100` | AND | `a & b` | 0 |
| `0101` | OR | `a \| b` | 0 |
| `0110` | XOR | `a ^ b` | 0 |
| `0111` | NOT | `~a` | 0 |
| `1000` | SHL | `a << 1` | original bit 7 |
| `1001` | SHR | `a >> 1` | original bit 0 |
| `1010` | ROL | rotate left (bit 7 → bit 0) | original bit 7 |
| `1011` | CMP | `a` (compare only) | `a > b` |
| `1100` | PASS | `a` | 0 |
| `1101` | CLR | `0` | 0 |
| `1110` | HLT | `0` (does nothing) | 0 |

## Flags

| Flag | Meaning |
|---|---|
| Z (zero) | result is 0; for CMP: `a == b` |
| C (carry) | carry / borrow as listed above |
| N (negative) | bit 7 of the result; for CMP: `a < b` |

## The ALU as a "syscall"

A **single instruction** covers every ALU operation. The operation code is not part of the instruction –
the programmer stores it in register **R1** beforehand, similar to how an operating system call
takes the service number in a register.

```
ALU    0100 aaaa bbbb dddd      Rd = Ra (operation R1[3:0]) Rb
```

| Bits | Meaning |
|---|---|
| `[15:12]` | opcode `0100` |
| `[11:8]` | first operand Ra |
| `[7:4]` | second operand Rb |
| `[3:0]` | destination register Rd |

### Example: 59 + 49

```
MVI  R1, ADD      ; 0x1100   R1 = 0x00 (ADD code)
MVI  R2, 59       ; 0x123B
MVI  R3, 49       ; 0x1331
ALU  R2, R3, R4   ; 0x4234   R4 = 59 + 49 = 108
```

### Why this design

- **One instruction for all operations** – saves opcode space, leaving more free opcodes for other instructions.
- **Three operands** – the result can go to a different register, the sources stay unchanged.
- **The operation can be chosen at run time** – a calculator, for example, just stores the selected operation
  in R1 and executes `ALU`, with no branching per operation.
- **In a loop, R1 is set only once**, before the loop.

R1 is therefore reserved for the operation code and must be set before the `ALU` instruction is used.

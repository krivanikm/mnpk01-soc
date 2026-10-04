---
title: Instruction set
---

# Instruction set architecture (ISA)

## Instruction format

Every instruction is a single 16-bit word:

```
 15    12 11     8 7      4 3      0
+--------+--------+--------+--------+
| opcode |  reg   |  data / operand |
+--------+--------+--------+--------+
```

- `opcode` `[15:12]` – instruction number (16 possible)
- `reg` `[11:8]` – destination register R0 – R15
- `[7:0]` – 8-bit constant or additional operands

## Implemented instructions

| Opcode | Name | Format | Operation | Cycles |
|---|---|---|---|---|
| `0x0` | NOP | `0000 xxxx xxxx xxxx` | nothing, only PC + 1 | 3 |
| `0x1` | MVI | `0001 rrrr dddd dddd` | `r[7:0] = d` | 3 |
| `0x2` | MOV | `0010 rrrr ssss xxxx` | `r[7:0] = s[7:0]` | 4 |
| `0x3` | MVIB | `0011 rrrr dddd dddd` | `r[15:8] = d` | 3 |

An unknown opcode behaves like NOP.

### 16-bit constant

A 16-bit value is loaded into a register with two instructions – MVI for the low byte and MVIB for the high byte:

```
MVI  R2, 0xAA   ; 0x12AA  -> R2[7:0]  = 0xAA
MVIB R2, 0x55   ; 0x3255  -> R2[15:8] = 0x55   => R2 = 0x55AA
```

## Proposed instructions

| Opcode | Name | Format | Operation |
|---|---|---|---|
| `0x4` | ALU | `0100 aaaa bbbb dddd` | `Rd = Ra (operation from R1) Rb` – see [ALU](alu.html) |

Further instructions (jumps, memory access, input/output) are being designed.

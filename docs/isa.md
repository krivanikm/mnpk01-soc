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
- `reg` `[11:8]` – register R0 – R15: the destination for MVI, MVIB and MOV, the first operand Ra for ALU, the jump condition for JMP
- `[7:0]` – 8-bit constant (MVI, MVIB) or two more register numbers (MOV, ALU)

## Implemented instructions

| Opcode | Name | Format | Operation | Cycles |
|---|---|---|---|---|
| `0x0` | NOP | `0000 xxxx xxxx xxxx` | nothing, only PC + 1 | 3 |
| `0x1` | MVI | `0001 rrrr dddd dddd` | `r[7:0] = d` | 3 |
| `0x2` | MOV | `0010 rrrr ssss xxxx` | `r = s` (all 16 bits) | 4 |
| `0x3` | MVIB | `0011 rrrr dddd dddd` | `r[15:8] = d` | 3 |
| `0x6` | JMP | `0110 cccc rrrr xxxx` | if condition `cccc` holds: `PC = Rr` | 5 taken / 3 not taken |
| `0x7` | ALU | `0111 aaaa bbbb dddd` | `Rd = Ra (operation from R1) Rb`, 16-bit – see [ALU](alu.html) | 4 |

Opcodes `0x4` (LOAD) and `0x5` (STORE) are reserved; until they are implemented they – like any unknown opcode – behave like NOP.

### 16-bit constant

A 16-bit value is loaded into a register with two instructions – MVI for the low byte and MVIB for the high byte:

```
MVI  R2, 0xAA   ; 0x12AA  -> R2[7:0]  = 0xAA
MVIB R2, 0x55   ; 0x3255  -> R2[15:8] = 0x55   => R2 = 0x55AA
```

## Jumps

```
JMP    0110 cccc rrrr xxxx      if condition cccc holds: PC = Rr
```

The target address is taken from register **Rr**, so a jump can reach any of the 65 536 addresses.
The address is loaded into the register beforehand, for example with MVI + MVIB.
One instruction covers all jumps – the condition is in bits `[11:8]`:

| `cccc` | Name | Jumps when | After `CMP Ra, Rb` it means |
|---|---|---|---|
| `0000` | JMP | always | – |
| `0001` | JZ | Z = 1 | Ra == Rb |
| `0010` | JNZ | Z = 0 | Ra != Rb |
| `0011` | JC | C = 1 | Ra > Rb |
| `0100` | JNC | C = 0 | Ra <= Rb |
| `0101` | JN | N = 1 | Ra < Rb |
| `0110` | JNN | N = 0 | Ra >= Rb |
| `0111` – `1111` | – | never (reserved) | – |

The flags are the ones stored by the last `ALU` instruction – not only CMP: after SUB or DEC, for example,
Z = 1 means the result reached zero, which is exactly what a counting loop needs.
Comparisons are unsigned.

### Example: 6 × 7 by repeated addition

```
MVI  R3, 7          ; 0x1307   what is added
MVI  R4, 6          ; 0x1406   counter
MVI  R5, 1          ; 0x1501
MVI  R14, 4         ; 0x1E04   address of the loop
loop:
MVI  R1, ADD        ; 0x1100
ALU  R3, R2, R2     ; 0x7322   R2 = R2 + 7
MVI  R1, SUB        ; 0x1101
ALU  R4, R5, R4     ; 0x7454   R4 = R4 - 1, Z = 1 at zero
JNZ  R14            ; 0x62E0   repeat while R4 != 0
                    ;          R2 = 42
```

## Planned instructions

Memory access (`0x4` LOAD, `0x5` STORE) and input/output are being designed.

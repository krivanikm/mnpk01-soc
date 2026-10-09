---
title: Inštrukčná sada
lang: sk
---

# Inštrukčná sada (ISA)

## Formát inštrukcie

Každá inštrukcia je jedno 16-bitové slovo:

```
 15    12 11     8 7      4 3      0
+--------+--------+--------+--------+
| opcode |  reg   |  dáta / operand |
+--------+--------+--------+--------+
```

- `opcode` `[15:12]` – číslo inštrukcie (16 možností)
- `reg` `[11:8]` – register R0 – R15: cieľ pri MVI, MVIB a MOV, prvý operand Ra pri ALU
- `[7:0]` – 8-bitová konštanta (MVI, MVIB) alebo čísla ďalších registrov (MOV, ALU)

## Implementované inštrukcie

| Opcode | Meno | Formát | Čo robí | Takty |
|---|---|---|---|---|
| `0x0` | NOP | `0000 xxxx xxxx xxxx` | nič, iba PC + 1 | 3 |
| `0x1` | MVI | `0001 rrrr dddd dddd` | `r[7:0] = d` | 3 |
| `0x2` | MOV | `0010 rrrr ssss xxxx` | `r = s` (celých 16 bitov) | 4 |
| `0x3` | MVIB | `0011 rrrr dddd dddd` | `r[15:8] = d` | 3 |
| `0x7` | ALU | `0111 aaaa bbbb dddd` | `Rd = Ra (operácia z R1) Rb`, 16-bitovo – pozri [ALU](alu.html) | 4 |

Opcode `0x4` (LOAD), `0x5` (STORE) a `0x6` (JMP) sú rezervované; kým nie sú implementované, správajú sa – rovnako ako neznámy opcode – ako NOP.

### 16-bitová konštanta

Do registra sa 16-bitová hodnota nahrá dvomi inštrukciami – MVI pre dolný a MVIB pre horný bajt:

```
MVI  R2, 0xAA   ; 0x12AA  -> R2[7:0]  = 0xAA
MVIB R2, 0x55   ; 0x3255  -> R2[15:8] = 0x55   => R2 = 0x55AA
```

## Plánované inštrukcie

Skoky (`0x6`), práca s pamäťou (`0x4`, `0x5`) a vstup/výstup sú vo fáze návrhu.

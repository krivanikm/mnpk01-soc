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
- `reg` `[11:8]` – register R0 – R15: cieľ pri MVI, MVIB a MOV, prvý operand Ra pri ALU, podmienka skoku pri JMP
- `[7:0]` – 8-bitová konštanta (MVI, MVIB) alebo čísla ďalších registrov (MOV, ALU)

## Implementované inštrukcie

| Opcode | Meno | Formát | Čo robí | Takty |
|---|---|---|---|---|
| `0x0` | NOP | `0000 xxxx xxxx xxxx` | nič, iba PC + 1 | 3 |
| `0x1` | MVI | `0001 rrrr dddd dddd` | `r[7:0] = d` | 3 |
| `0x2` | MOV | `0010 rrrr ssss xxxx` | `r = s` (celých 16 bitov) | 4 |
| `0x3` | MVIB | `0011 rrrr dddd dddd` | `r[15:8] = d` | 3 |
| `0x6` | JMP | `0110 cccc rrrr xxxx` | ak platí podmienka `cccc`: `PC = Rr` | 5 so skokom / 3 bez |
| `0x7` | ALU | `0111 aaaa bbbb dddd` | `Rd = Ra (operácia z R1) Rb`, 16-bitovo – pozri [ALU](alu.html) | 4 |

Opcode `0x4` (LOAD) a `0x5` (STORE) sú rezervované; kým nie sú implementované, správajú sa – rovnako ako neznámy opcode – ako NOP.

### 16-bitová konštanta

Do registra sa 16-bitová hodnota nahrá dvomi inštrukciami – MVI pre dolný a MVIB pre horný bajt:

```
MVI  R2, 0xAA   ; 0x12AA  -> R2[7:0]  = 0xAA
MVIB R2, 0x55   ; 0x3255  -> R2[15:8] = 0x55   => R2 = 0x55AA
```

## Skoky

```
JMP    0110 cccc rrrr xxxx      ak platí podmienka cccc: PC = Rr
```

Cieľová adresa sa berie z registra **Rr**, takže skok dosiahne ktorúkoľvek zo 65 536 adries.
Adresu treba do registra nahrať vopred, napríklad cez MVI + MVIB.
Na všetky skoky stačí jedna inštrukcia – podmienka je v bitoch `[11:8]`:

| `cccc` | Meno | Skočí, keď | Po `CMP Ra, Rb` to znamená |
|---|---|---|---|
| `0000` | JMP | vždy | – |
| `0001` | JZ | Z = 1 | Ra == Rb |
| `0010` | JNZ | Z = 0 | Ra != Rb |
| `0011` | JC | C = 1 | Ra > Rb |
| `0100` | JNC | C = 0 | Ra <= Rb |
| `0101` | JN | N = 1 | Ra < Rb |
| `0110` | JNN | N = 0 | Ra >= Rb |
| `0111` – `1111` | – | nikdy (rezerva) | – |

Flagy sú tie, ktoré uložila posledná inštrukcia `ALU` – nielen CMP: napríklad po SUB alebo DEC
znamená Z = 1, že výsledok klesol na nulu, čo je presne to, čo potrebuje slučka s počítadlom.
Porovnanie je bez znamienka.

### Príklad: 6 × 7 opakovaným sčítaním

```
MVI  R3, 7          ; 0x1307   čo sa pripočítava
MVI  R4, 6          ; 0x1406   počítadlo
MVI  R5, 1          ; 0x1501
MVI  R14, 4         ; 0x1E04   adresa začiatku slučky
slučka:
MVI  R1, ADD        ; 0x1100
ALU  R3, R2, R2     ; 0x7322   R2 = R2 + 7
MVI  R1, SUB        ; 0x1101
ALU  R4, R5, R4     ; 0x7454   R4 = R4 - 1, pri nule Z = 1
JNZ  R14            ; 0x62E0   opakuj, kým R4 != 0
                    ;          R2 = 42
```

## Plánované inštrukcie

Práca s pamäťou (`0x4` LOAD, `0x5` STORE) a vstup/výstup sú vo fáze návrhu.

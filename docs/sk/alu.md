---
title: ALU
lang: sk
---

# ALU – aritmeticko-logická jednotka

Súbor: `rtl/alu/alu.sv`

Kombinačná 8-bitová ALU. Má dva operandy `a` a `b`, 4-bitový kód operácie `op`
a nastavuje tri príznaky (flagy).

## Operácie

| Kód | Meno | Výsledok | Carry (C) |
|---|---|---|---|
| `0000` | ADD | `a + b` | prenos z bitu 7 |
| `0001` | SUB | `a - b` | výpožička (`a < b`) |
| `0010` | INC | `a + 1` | prenos z bitu 7 |
| `0011` | DEC | `a - 1` | výpožička (`a < 1`) |
| `0100` | AND | `a & b` | 0 |
| `0101` | OR | `a \| b` | 0 |
| `0110` | XOR | `a ^ b` | 0 |
| `0111` | NOT | `~a` | 0 |
| `1000` | SHL | `a << 1` | pôvodný bit 7 |
| `1001` | SHR | `a >> 1` | pôvodný bit 0 |
| `1010` | ROL | rotácia doľava (bit 7 → bit 0) | pôvodný bit 7 |
| `1011` | CMP | `a` (iba porovnanie) | `a > b` |
| `1100` | PASS | `a` | 0 |
| `1101` | CLR | `0` | 0 |
| `1110` | HLT | `0` (nerobí nič) | 0 |

## Flagy

| Flag | Význam |
|---|---|
| Z (zero) | výsledok je 0; pri CMP: `a == b` |
| C (carry) | prenos / výpožička podľa tabuľky vyššie |
| N (negative) | bit 7 výsledku; pri CMP: `a < b` |

## ALU ako „syscall“

Na všetky operácie ALU stačí **jedna inštrukcia**. Kód operácie nie je priamo v inštrukcii,
ale programátor ho vopred uloží do registra **R1** – podobne ako sa pri systémovom volaní
v operačnom systéme ukladá číslo služby do registra.

```
ALU    0100 aaaa bbbb dddd      Rd = Ra (operácia R1[3:0]) Rb
```

| Bity | Význam |
|---|---|
| `[15:12]` | opcode `0100` |
| `[11:8]` | prvý operand Ra |
| `[7:4]` | druhý operand Rb |
| `[3:0]` | cieľový register Rd |

### Príklad: 59 + 49

```
MVI  R1, ADD      ; 0x1100   R1 = 0x00 (kód ADD)
MVI  R2, 59       ; 0x123B
MVI  R3, 49       ; 0x1331
ALU  R2, R3, R4   ; 0x4234   R4 = 59 + 49 = 108
```

### Prečo takto

- **Jedna inštrukcia pre všetky operácie** – šetrí miesto v opcode, ostáva viac voľných opcode pre iné inštrukcie.
- **Tri operandy** – výsledok môže ísť do iného registra, zdroje ostanú nezmenené.
- **Operácia sa dá vybrať za behu programu** – napríklad kalkulačka iba uloží zvolenú operáciu do R1
  a zavolá `ALU`, bez vetvenia podľa operácie.
- **V slučke sa R1 nastaví iba raz**, pred slučkou.

R1 je preto vyhradený na kód operácie a pred použitím inštrukcie `ALU` musí byť nastavený.

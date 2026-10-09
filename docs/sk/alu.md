---
title: ALU
lang: sk
---

# ALU – aritmeticko-logická jednotka

Súbor: `rtl/alu/alu.sv`

Kombinačná 16-bitová ALU. Má dva 16-bitové operandy `a` a `b`, 4-bitový kód operácie `op`
a nastavuje tri príznaky (flagy).

## Operácie

| Kód | Meno | Výsledok | Carry (C) |
|---|---|---|---|
| `0000` | ADD | `a + b` | prenos z bitu 15 |
| `0001` | SUB | `a - b` | výpožička (`a < b`) |
| `0010` | INC | `a + 1` | prenos z bitu 15 |
| `0011` | DEC | `a - 1` | výpožička (`a < 1`) |
| `0100` | AND | `a & b` | 0 |
| `0101` | OR | `a \| b` | 0 |
| `0110` | XOR | `a ^ b` | 0 |
| `0111` | NOT | `~a` | 0 |
| `1000` | SHL | `a << 1` | pôvodný bit 15 |
| `1001` | SHR | `a >> 1` | pôvodný bit 0 |
| `1010` | ROL | rotácia doľava (bit 15 → bit 0) | pôvodný bit 15 |
| `1011` | CMP | `a` (iba porovnanie) | `a > b` |
| `1100` | PASS | `a` | 0 |
| `1101` | CLR | `0` | 0 |
| `1110` | HLT | `0` (nerobí nič) | 0 |

## Flagy

| Flag | Význam |
|---|---|
| Z (zero) | výsledok je 0; pri CMP: `a == b` |
| C (carry) | prenos / výpožička podľa tabuľky vyššie |
| N (negative) | bit 15 výsledku (znamienko); pri CMP: `a < b` |

## ALU ako „syscall“

Na všetky operácie ALU stačí **jedna inštrukcia**. Kód operácie nie je priamo v inštrukcii,
ale programátor ho vopred uloží do registra **R1** – podobne ako sa pri systémovom volaní
v operačnom systéme ukladá číslo služby do registra.

```
ALU    0111 aaaa bbbb dddd      Rd = Ra (operácia R1[3:0]) Rb
```

| Bity | Význam |
|---|---|
| `[15:12]` | opcode `0111` |
| `[11:8]` | prvý operand Ra |
| `[7:4]` | druhý operand Rb |
| `[3:0]` | cieľový register Rd |

### Príklad: 59 + 49

```
MVI  R1, ADD      ; 0x1100   R1 = 0x00 (kód ADD)
MVI  R2, 59       ; 0x123B
MVI  R3, 49       ; 0x1331
ALU  R2, R3, R4   ; 0x7234   R4 = 59 + 49 = 108
```

### Príklad: 16-bitové sčítanie

```
MVI  R1, ADD      ; 0x1100
MVI  R2, 0xFF     ; 0x12FF
MVIB R2, 0x12     ; 0x3212   R2 = 0x12FF
MVI  R3, 1        ; 0x1301
ALU  R2, R3, R4   ; 0x7234   R4 = 0x12FF + 1 = 0x1300 (prenos z dolného do horného bajtu)
```

### Prečo takto

- **Jedna inštrukcia pre všetky operácie** – šetrí miesto v opcode, ostáva viac voľných opcode pre iné inštrukcie.
- **Tri operandy** – výsledok môže ísť do iného registra, zdroje ostanú nezmenené.
- **Operácia sa dá vybrať za behu programu** – napríklad kalkulačka iba uloží zvolenú operáciu do R1
  a zavolá `ALU`, bez vetvenia podľa operácie.
- **V slučke sa R1 nastaví iba raz**, pred slučkou.

R1 je preto vyhradený na kód operácie a pred použitím inštrukcie `ALU` musí byť nastavený.

## Realizácia v hardvéri

Bity `R1[3:0]` idú z register file **priamo drôtom** na vstup `op` ALU – register file má na to samostatný
výstup `r1_out`. ALU teda vždy vidí aktuálny obsah R1 a na prečítanie operácie netreba žiadny takt navyše.

Register file má jeden 16-bitový čítací port, preto control unit číta operandy postupne:

| Takt | Stav | Čo sa deje |
|---|---|---|
| 1 | `S_FETCH` | načítanie inštrukcie, PC + 1 |
| 2 | `S_DECODE` | adresa registra ← Ra |
| 3 | `S_ALU_A` | Ra sa uloží do registra `alu_a` (operand A), adresa registra ← Rb |
| 4 | `S_ALU_B` | ALU dostane A = uložený Ra, B = Rb priamo z register file, výsledok je hotový v tom istom takte (ALU je kombinačná); pripraví sa zápis do Rd a flagy |

Celý 16-bitový výsledok sa zapíše do Rd na ďalšej hrane hodín (`write_reg_en = 11`).
Flagy Z, C, N si control unit uloží po každej inštrukcii `ALU`.

- **CMP** (`1011`) iba nastaví flagy a do Rd **nezapisuje**.
- Unárne operácie (INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR) ignorujú Rb.
- Rd môže byť ten istý register ako Ra alebo Rb (`ALU R2, R2, R2` zdvojnásobí R2).
- Rd môže byť dokonca **R1** – ALU tak vypočíta kód operácie pre ďalšiu inštrukciu `ALU`.

---
title: Architektúra
lang: sk
---

# Architektúra

```
              +-----------------+
   SPI flash  |  Pamäť programu | 16-bit inštrukcia
  ----------> |     (BRAM)      | ------------------+
              +-----------------+                   |
                       ^ 16-bit adresa              v
              +-----------------+          +-----------------+
              | Program counter | <------- |  Control unit   |
              +-----------------+   skok   +-----------------+
                                             |      |      ^
                                  riadenie   |      |      | flagy Z/C/N
                                             v      v      |
                                  +---------------+  +-----------+
                                  | Register file |<>|    ALU    |
                                  |  16 x 16 bit  |  |  16 bit   |
                                  +---------------+  +-----------+
```

Control unit riadi všetky bloky priamo, nie je medzi nimi žiadna zbernicová jednotka.
ALU berie kód operácie priamo z registra R1 (pozri [ALU](alu.html)).

## Program counter

Súbor: `rtl/program-counter/programcounter.v`

16-bitový register s adresou aktuálnej inštrukcie.

| Signál | Smer | Význam |
|---|---|---|
| `clk`, `rst` | vstup | hodiny, asynchrónny reset (PC = 0) |
| `pc_inc` | vstup | PC = PC + 1 |
| `pc_load` | vstup | PC = `d_in` (skok) |
| `d_in[15:0]` | vstup | cieľová adresa skoku |
| `pc[15:0]` | výstup | aktuálna adresa |

Ak sú `pc_inc` aj `pc_load` naraz 1, vyhrá `pc_inc` – skok sa nevykoná.
Control unit ich nikdy nenastaví naraz: `pc_inc` iba v `S_FETCH`, `pc_load` iba v `S_JMP`.
Program counter je súčasťou `system_top`, jeho výstup `pc` je adresa pre pamäť programu.

## Register file

Súbor: `rtl/registers/register_file.sv`

16 registrov po 16 bitoch. Z registra sa vždy **číta celé 16-bitové slovo**.
Zápis má dva povoľovacie bity, jeden pre každý bajt, takže control unit vie zapísať dolný bajt (MVI),
horný bajt (MVIB) alebo celé slovo (MOV, ALU).

| Signál | Smer | Význam |
|---|---|---|
| `clk`, `rst_n` | vstup | hodiny, asynchrónny reset (aktívny v 0), vynuluje všetky registre |
| `addr[3:0]` | vstup | číslo registra R0 – R15 |
| `d_in[15:0]` | vstup | zapisované dáta |
| `write_en[1:0]` | vstup | zápis na nábežnej hrane hodín: `01` = dolný bajt `[7:0]`, `10` = horný bajt `[15:8]`, `11` = celé slovo |
| `q_out[15:0]` | výstup | čítaný register (kombinačne, hneď po zmene adresy) |
| `r1_out[3:0]` | výstup | `R1[3:0]`, ide priamo do ALU ako kód operácie |

## Control unit

Súbor: `rtl/control-unit/controlunit.sv`

Konečný automat (FSM), ktorý pre každú inštrukciu prechádza stavmi:

```
S_FETCH -> S_DECODE -> stavy vykonania -> S_FETCH
```

| Stav | Čo sa deje |
|---|---|
| `S_FETCH` | načítanie inštrukcie z pamäte programu |
| `S_DECODE` | dekódovanie opcode, výber ďalšieho stavu |
| `S_NOP` | prázdny takt, kým sa posunie PC |
| `S_MVI_WRITE` | zápis konštanty do registra (MVI aj MVIB) |
| `S_MOV_LATCH` | prečítanie zdrojového registra do pomocného registra |
| `S_MOV_WRITE` | zápis do cieľového registra |
| `S_ALU_A` | uloženie operandu Ra, výber Rb |
| `S_ALU_B` | zápis výsledku ALU do Rd (okrem CMP), uloženie flagov |
| `S_JMP` | prečíta sa cieľový register; ak platí podmienka: `pc_addr` = cieľ, `pc_load` = 1 |
| `S_JMP_WAIT1` | PC na konci taktu prevezme cieľovú adresu |
| `S_JMP_WAIT2` | pamäť na konci taktu zachytí cieľovú adresu |

### Pravidlo časovania

Pamäť programu vo FPGA (BRAM) je synchrónna: dáta pre novú adresu prídu až o takt neskôr.
`S_FETCH` preto uloží celú inštrukciu do inštrukčného registra a hneď nastaví `pc_inc`;
ďalšie stavy už používajú len inštrukčný register, takže pamäť medzitým načítava ďalšiu inštrukciu:

| Takt | Stav | |
|---|---|---|
| 1 | `S_FETCH` | inštrukcia → inštrukčný register, `pc_inc` = 1 |
| 2 | `S_DECODE` | PC sa na konci taktu posunie |
| 3 | vykonanie | pamäť na konci taktu zachytí novú adresu |
| 4 | `S_FETCH` | ďalšia inštrukcia je platná |

Pravidlo: `pc_inc` sa nastavuje **iba** v `S_FETCH` a každá inštrukcia trvá **aspoň 3 takty**
(preto NOP ide cez `S_NOP`).

### Časovanie skoku

Výstupy control unit sú registrované a pamäť je synchrónna, preto skok, ktorý sa vykoná, potrebuje dva takty navyše,
kým sa dá načítať inštrukcia z cieľovej adresy T:

| Takt | Stav | Čo sa deje | PC | Pamäť zachytí |
|---|---|---|---|---|
| 1 | `S_FETCH` | JMP → inštrukčný register, `pc_inc` = 1 | J | J |
| 2 | `S_DECODE` | adresa registra ← Rr | J → J+1 | J |
| 3 | `S_JMP` | prečíta sa cieľ, `pc_load` = 1 (ak platí podmienka) | J+1 | J+1 |
| 4 | `S_JMP_WAIT1` | PC na konci taktu prevezme cieľ | J+1 → T | J+1 |
| 5 | `S_JMP_WAIT2` | | T | **T** |
| 6 | `S_FETCH` | inštrukcia z adresy T je platná | T | |

Skok, ktorý sa **nevykoná**, ide z `S_JMP` rovno späť do `S_FETCH` – PC už ukazuje za JMP,
takže trvá 3 takty ako ktorákoľvek iná krátka inštrukcia.

Podmienku vyhodnocuje kombinačný signál `cond_ok` z `ir[11:8]` a uložených flagov.

## Schémy

RTL schémy všetkých blokov vygenerované nástrojom Yosys sú na stránke [Schémy](schematics.html).

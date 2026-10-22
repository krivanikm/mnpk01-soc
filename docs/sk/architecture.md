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
              +-----------------+          +-----------------+   adresa, dáta  +-----------------+
              | Program counter | <------- |  Control unit   | <-------------> |  Dátová RAM     |
              +-----------------+   skok   +-----------------+   LOAD/STORE    | 4K x 16 (BRAM)  |
                                             |      |      ^                   +-----------------+
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

## Dátová RAM

Súbor: `rtl/memory/data_ram.sv`

4096 slov po 16 bitoch, realizovaných v blokovej pamäti FPGA (na Tang Nano 9K 4 bloky BSRAM).
Je **synchrónna**: zápis aj čítanie prebieha na nábežnej hrane hodín, prečítané dáta sú teda platné takt po adrese.
Pole pamäte zámerne nemá reset – s resetom by ho syntéza nevedela namapovať na blokovú pamäť
a použila by namiesto nej 65 536 klopných obvodov.

| Signál | Smer | Význam |
|---|---|---|
| `clk` | vstup | hodiny |
| `addr[11:0]` | vstup | adresa slova (dolných 12 bitov adresy z registra) |
| `din[15:0]` | vstup | zapisované dáta |
| `we` | vstup | 1 = na nábežnej hrane zapíš `din` na `addr` |
| `dout[15:0]` | výstup | dáta z `addr`, platné o takt neskôr |

RAM vidí len dolných 12 bitov adresy; oblasť I/O `0xFF00` – `0xFFFF` sa dekóduje v `system_top` pred RAM (pozri [Vstup a výstup](#vstup-a-výstup)).

## Vstup a výstup

Vstup a výstup sú **mapované do pamäte**: tie isté inštrukcie `LOAD` a `STORE`, ktoré pristupujú k RAM, sa dostanú aj k portom, samostatná I/O inštrukcia neexistuje.
`system_top` dekóduje celú 16-bitovú adresu prístupu do pamäte:

```
io_sel = (mem_addr[15:8] == 8'hFF)          adresa je v 0xFF00 – 0xFFFF
zápis do RAM = mem_we && !io_sel             STORE do oblasti I/O sa nikdy nezapíše do RAM
io_out <= mem_wdata   keď mem_we && mem_addr == 16'hFF00
```

| Port | Smer | Adresa | Popis |
|---|---|---|---|
| `io_out[15:0]` | výstup | `0xFF00` (STORE) | 16-bitový register, po resete 0 |
| `io_in[15:0]` | vstup | `0xFF01` (LOAD) | hodnota pinov, čítaná cez multiplexor |

**Čítanie.** RAM je synchrónna, takže LOAD čaká jeden takt na dáta (`S_LOAD_WAIT`). Vstupný port musí odpovedať s rovnakým oneskorením:
`system_top` zaregistruje `io_sel` aj `io_in` v takte, v ktorom RAM zachytáva adresu, a potom vyberie medzi výstupom RAM a portom:

```
w_mem_rdata = io_sel_q ? io_in_q : ram_dout
```

Bez toho by CU prečítala predošlú hodnotu portu namiesto aktuálnej.
Testy `io_read` a `io_read_ram_mix` kontrolujú presne toto, vrátane striedavých LOAD z portu a z RAM.

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
| `S_STORE_ADDR` | adresový register Ra sa prečíta do `mem_addr`, vyberie sa dátový register Rs |
| `S_STORE_DATA` | Rs ide do `mem_wdata`, `mem_we` = 1 – RAM zapíše na konci ďalšieho taktu |
| `S_LOAD_ADDR` | adresový register Ra sa prečíta do `mem_addr` |
| `S_LOAD_WAIT` | RAM na konci taktu zachytí adresu |
| `S_LOAD_WRITE` | dáta z RAM sa zapíšu do Rd |

Register stavu má 5 bitov (stavov je 16, ďalší by sa do 4 bitov už nezmestil).

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

### Časovanie LOAD / STORE

Register file má jeden čítací port, preto STORE číta svoje dva registre (adresu a dáta) postupne.
LOAD musí počkať jeden takt na synchrónnu RAM:

| Takt | STORE | LOAD |
|---|---|---|
| 1 | `S_FETCH` | `S_FETCH` |
| 2 | `S_DECODE` – adresa registra ← Ra | `S_DECODE` – adresa registra ← Ra |
| 3 | `S_STORE_ADDR` – `mem_addr` ← Ra, adresa registra ← Rs | `S_LOAD_ADDR` – `mem_addr` ← Ra |
| 4 | `S_STORE_DATA` – `mem_wdata` ← Rs, `mem_we` = 1 | `S_LOAD_WAIT` – RAM zachytí adresu |
| 5 | (ďalší `S_FETCH`, RAM zapíše na konci taktu) | `S_LOAD_WRITE` – Rd ← `mem_rdata` |

STORE trvá **4 takty**, LOAD **5 taktov**.

## Schémy

RTL schémy všetkých blokov vygenerované nástrojom Yosys sú na stránke [Schémy](schematics.html).

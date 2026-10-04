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
                                  |  16 x 16 bit  |  |   8 bit   |
                                  +---------------+  +-----------+
```

Control unit riadi všetky bloky priamo, nie je medzi nimi žiadna zbernicová jednotka.

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

## Register file

Súbor: `rtl/registers/register_file.sv`

16 registrov po 16 bitoch. Dátová cesta je 8-bitová, preto sa do registra zapisuje aj z neho číta
**po bajtoch** – signál `high_b` vyberá horný (`[15:8]`) alebo dolný (`[7:0]`) bajt.

| Signál | Smer | Význam |
|---|---|---|
| `clk`, `rst_n` | vstup | hodiny, asynchrónny reset (aktívny v 0), vynuluje všetky registre |
| `addr[3:0]` | vstup | číslo registra R0 – R15 |
| `high_b` | vstup | 1 = horný bajt, 0 = dolný bajt |
| `d_in[7:0]` | vstup | zapisovaný bajt |
| `write_en` | vstup | zápis na nábežnej hrane hodín |
| `q_out[7:0]` | výstup | čítaný bajt (kombinačne, hneď po zmene adresy) |

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

### Pravidlo časovania

`pc_inc` je registrovaný výstup, takže PC sa posunie až takt po stave, ktorý ho nastavil.
Preto sa `pc_inc` musí nastaviť najneskôr jeden stav **pred** návratom do `S_FETCH`, inak by sa načítala
tá istá inštrukcia znova.

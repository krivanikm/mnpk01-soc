---
title: FPGA
---

# FPGA

Procesor beží na doske **Tang Nano 9K** (Gowin GW1NR-9, oscilátor 27 MHz, 32 Mbit SPI flash, 6 LED). Používa sa rovnaké RTL, ktoré sa overuje v simulátore:
nový je len malý top modul okolo neho.

## Postup zostavenia

Celý postup je open source:

| Krok | Nástroj |
|---|---|
| syntéza | Yosys (`synth_gowin`) |
| rozmiestnenie a prepojenie | nextpnr (himbaechel, Gowin) |
| bitstream | Apicula (`gowin_pack`) |
| programovanie | openFPGALoader |

V priečinku `fpga/`:

```
make          # bitstream top.fs
make flash    # nahrá ho do FPGA (SRAM)
```

Správa nextpnr udáva maximálne hodiny okolo 40 MHz, hodiny na doske majú 27 MHz. Návrh zaberie 5 z 26 blokov pamäte.

## Top modul

Súbor: `fpga/top.sv`. Obsahuje len „lepidlo“ okolo `system_top`:

| Časť | Čo robí |
|---|---|
| reset | 4-bitové počítadlo drží `rst` v jednotke 15 taktov po zapnutí (doska nemá resetovacie tlačidlo) |
| pamäť programu | pole 256 × 16 bitov naplnené zo `prog.hex` cez `$readmemh`, adresované `pc` |
| synchronizácia vstupu | dva registre za sebou pre spínače, aby CPU nikdy nevidel signál, ktorý sa mení práve na hrane hodín |
| porty | negovaná hodnota spínačov ide do dolného bajtu `io_in`, dolný bajt `io_out` ide na LED |

## Vstup a výstup na doske

| Adresa | Prístup | Hardvér |
|---|---|---|
| `0xFF00` | `STORE` | `io_out[7:0]` na 8 externých LED (každá cez rezistor 330 Ω) |
| `0xFF01` | `LOAD` | `io_in[7:0]` z 8-polohového DIP spínača (ON = 1, piny majú pull-up) |

| Signál | Piny |
|---|---|
| hodiny | 52 |
| DIP spínač, bity 0 – 7 | 25, 26, 27, 28, 29, 30, 36, 37 |
| LED, bity 0 – 7 | 33, 34, 40, 35, 41, 42, 51, 53 |

Priradenie pinov je v `fpga/tang9k.cst`.

## Programy, ktoré bežali na doske

Kým neexistoval I/O port, procesor sa na doske skúšal cez wrapper, ktorý sledoval zápisy do registra R12 a používal ich ako výstupný kanál.
RTL procesora ostalo nezmenené. Násobenie ani delenie neexistuje, obe sa robia posunmi a sčítaním.

| Program | Popis |
|---|---|
| číslice π | spigot algoritmus, procesor vypočítal 980 a neskôr 1950 číslic, zobrazené na 1,14″ displeji |
| Mandelbrotova množina | 135 × 240 pixelov, 16-bitová pevná rádová čiarka (3.12), násobenie posunom a sčítaním; jeden snímok trvá približne 7 sekúnd a obraz sa približuje do zvoleného bodu |

Tieto dva programy a ovládač displeja som vytvoril s pomocou Claude, AI asistenta od Anthropic.

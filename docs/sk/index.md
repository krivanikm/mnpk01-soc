---
title: Úvod
lang: sk
---

# MNPK-01

**MNPK-01** je 16-bitový mikrokontrolér navrhnutý od úrovne logiky v jazyku SystemVerilog až po fyzický plošný spoj.
Cieľom projektu je ukázať celý reťazec návrhu počítača: vlastnú inštrukčnú sadu (ISA), jej realizáciu v hardvéri,
overenie simuláciou, nasadenie na FPGA a nakoniec vlastnú dosku plošných spojov.

## Základné parametre

| Parameter | Hodnota |
|---|---|
| Šírka dát | 16 bitov (registre, ALU) |
| Šírka adresy | 16 bitov |
| Šírka inštrukcie | 16 bitov (1 inštrukcia = 1 slovo) |
| Registre | 16 × 16 bitov (R0 – R15) |
| Dátová pamäť | RAM 4096 × 16 bitov (bloková pamäť FPGA), adresovaná po slovách |
| Architektúra | Harvardská – oddelená pamäť programu a dát |
| Vstup / výstup | memory-mapped na `0xFF00` – `0xFFFF`: 16-bitový výstupný port, 16-bitový vstupný port |
| Program | BRAM vo FPGA, naplnená z hex súboru (plánované: pri štarte načítaná z externej SPI flash) |

## Stav projektu

| Časť | Stav |
|---|---|
| ALU | hotová, 16-bitová, 14 operácií, ~40 miliónov kontrol (hraničné a náhodné hodnoty) |
| Register file | hotový |
| Program counter | hotový, zapojený v `system_top` |
| Control unit | fungujú všetky 8 inštrukcie (NOP, MVI, MOV, MVIB, LOAD, STORE, JMP, ALU), aj s BRAM vo FPGA |
| ALU inštrukcia (cez register R1) | hotová |
| Skoky (JMP + 6 podmienok) | hotové – fungujú slučky aj rozhodovanie |
| LOAD/STORE + dátová RAM | hotové – fungujú polia v pamäti |
| Automatické testy | 26/26 prechádza v oboch režimoch pamäte |
| I/O port | hotový – `STORE` na `0xFF00` ovláda výstup, `LOAD` z `0xFF01` číta vstup |
| Vysielač UART | hotový – `STORE` na `0xFF01` odošle bajt, `LOAD` z `0xFF02` číta `busy`, na doske beží „Hello World“ |
| Prijímač UART, nové inštrukcie, assembler | plánované |
| Syntéza (Yosys) | návrh sa zmestí do Tang Nano 9K, zaberie ~14 % LUT a 4 z 26 blokov pamäte, pozri [Schémy](schematics.html) |
| FPGA | hotové – procesor beží na Tang Nano 9K (LED, spínače a UART cez I/O port), pozri [FPGA](fpga.html) |
| Plošný spoj | plánované |

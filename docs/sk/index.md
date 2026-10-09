---
title: Úvod
lang: sk
---

# MNPK-01

**MNPK-01** je 16-bitový mikropočítač navrhnutý od úrovne logiky v jazyku SystemVerilog až po fyzický plošný spoj.
Cieľom projektu je ukázať celý reťazec návrhu počítača: vlastnú inštrukčnú sadu (ISA), jej realizáciu v hardvéri,
overenie simuláciou, nasadenie na FPGA a nakoniec vlastnú dosku plošných spojov.

## Základné parametre

| Parameter | Hodnota |
|---|---|
| Šírka dát | 16 bitov (registre, ALU) |
| Šírka adresy | 16 bitov |
| Šírka inštrukcie | 16 bitov (1 inštrukcia = 1 slovo) |
| Registre | 16 × 16 bitov (R0 – R15) |
| Architektúra | Harvardská – oddelená pamäť programu a dát |
| Program | BRAM vo FPGA, pri štarte načítaná z externej SPI flash |

## Stav projektu

| Časť | Stav |
|---|---|
| ALU | hotová, 16-bitová, ~40 miliónov testov (hraničné a náhodné hodnoty) |
| Register file | hotový |
| Program counter | hotový (zatiaľ nie je zapojený v `system_top`) |
| Control unit | rozpracovaná – fungujú NOP, MVI, MVIB, MOV a ALU (16-bitovo), aj s BRAM vo FPGA |
| ALU inštrukcia (cez register R1) | hotová, prechádza 13 automatických testov |
| Skoky, LOAD/STORE, RAM, I/O | v návrhu |
| FPGA, plošný spoj | plánované |

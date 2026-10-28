---
title: Simulácia
lang: sk
---

# Simulácia a testovanie

Návrh sa overuje simulátorom **Verilator**, testbenche sú napísané v C++.

## Test ALU

Súbor: `rtl/alu/sim_main.cpp`

Pri 16-bitových operandoch je kombinácií priveľa na to, aby sa vyskúšali všetky (65536 × 65536 na operáciu), preto pre každú operáciu testuje:

1. všetkých 65536 hodnôt `a` s hraničnými hodnotami `b` (`0`, `1`, `0x7F`, `0x80`, `0xFF`, `0x100`, `0x7FFF`, `0x8000`, `0xFFFF`, …),
2. hraničné hodnoty `a` so všetkými 65536 hodnotami `b`,
3. milión náhodných dvojíc (s pevným semienkom, takže test je vždy rovnaký).

Spolu je to asi **40 miliónov kontrol**; výsledok aj flagy sa porovnajú s hodnotami vypočítanými v C++.
Spúšťa sa príkazom `make alu` v `rtl/`.

## Test celého systému

Súbory: `rtl/tb_full_system.cpp`, `rtl/tests/*.txt`

Každý test je krátky program vo vlastnom textovom súbore spolu s očakávaným výsledkom:

```
1100    ; MVI R1, 0      (ADD)
123B    ; MVI R2, 59
1331    ; MVI R3, 49
7234    ; ALU R2, R3, R4
expect r2=59 r3=49 r4=108
```

Testbench program nahrá, spustí a na konci porovná **skutočný obsah všetkých 16 registrov**
s očakávanými hodnotami. Register, ktorý nie je uvedený, musí byť 0 – odhalí sa tak aj zápis do zlého registra.
Kontroluje aj konečnú hodnotu PC a zastaví program, ktorý sa zasekne.
Test môže skontrolovať aj dátovú RAM, napr. `expect mem[0x10]=42` (kontrolujú sa iba uvedené adresy).
I/O porty fungujú rovnako: riadok `io_in 0xA5C3` nastaví vstupné piny a `expect io_out=0x1234` skontroluje na konci testu výstupný register (test, ktorý `io_out` neuvedie, očakáva 0, takže sa zachytí aj náhodný zápis do portu).

Každý test sa spustí v **oboch režimoch pamäte**:

| Režim | Správanie |
|---|---|
| kombinačná | inštrukcia je k dispozícii okamžite |
| synchrónna (BRAM) | ako skutočná bloková pamäť vo FPGA: dáta prídu takt po adrese |

Program counter je skutočný hardvér (`programcounter.v` v `system_top`); testbench emuluje iba pamäť programu a podáva inštrukciu z adresy `pc`.

### Testy

| Test | Čo overuje |
|---|---|
| `basic` | MVI, MVIB, MOV, NOP spolu |
| `mvi_mvib` | 16-bitové konštanty, registre R0 a R15, prepísanie hodnoty |
| `mov` | MOV kopíruje celých 16 bitov, reťazenie, MOV sám na seba |
| `nop_unimplemented` | NOP a neznáme opcode (`0x8` – `0xF`) procesor nezaseknú |
| `alu_add` | 59 + 49 = 108 |
| `alu_ops` | SUB, AND, OR, XOR, zmena operácie v R1 medzi inštrukciami |
| `alu_sub_borrow` | odčítanie pod nulu, odčítanie do nuly |
| `alu_cmp` | CMP nezapisuje do Rd |
| `alu_rd_eq_src` | cieľ je ten istý register ako zdroj |
| `alu_unary` | INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR |
| `alu_high_byte` | ALU použije aj horné bajty operandov a prepíše celý Rd |
| `alu_carry16` | prenos / výpožička medzi dolným a horným bajtom, pretečenie 16 bitov |
| `alu_op_runtime` | ALU zapíše do R1 a tým vyberie ďalšiu operáciu |
| `jmp_always` | nepodmienený skok dopredu aj dozadu, preskočené inštrukcie sa nevykonajú |
| `jmp_cond` | všetkých 6 podmienok po CMP, každá raz skočí a raz nie; neznáma podmienka neskočí nikdy |
| `jmp_loop` | slučka: 6 × 7 = 42 opakovaným sčítaním, JNZ podľa flagu Z zo SUB |
| `mem_store_load` | STORE a LOAD späť, ten istý register ako adresa aj dáta, LOAD do adresového registra |
| `mem_back_to_back` | LOAD hneď po STORE a naopak, výmena dvoch hodnôt v pamäti |
| `mem_array` | pole 5 čísel v RAM sčítané v slučke (LOAD + JMP) = 150 |
| `io_write` | `STORE` na `0xFF00` nastaví `io_out` a nezapíše sa do RAM (inak by padlo na `0x0F00`) |
| `io_unmapped` | `STORE` na `0xFF01` (dátový port UART) nezmení `io_out` ani RAM |
| `io_and_ram` | zápis do portu a zápis do RAM sa nepomiešajú |
| `io_read` | `LOAD` z `0xFF01` vráti `io_in` |
| `io_read_ram_mix` | striedavé `LOAD` z portu a z RAM, výber portu nezostane zaseknutý |
| `uart_send_busy` | `STORE` na `0xFF01` spustí UART (`busy` = 1 pri nasledujúcom `LOAD` z `0xFF02`) a nič sa nezapíše do RAM |
| `uart_status_idle` | `LOAD` z `0xFF02` pred akýmkoľvek odoslaním vráti `busy` = 0 |

### Spustenie

V priečinku `rtl/`:

```
make test        # všetky testy, oba režimy pamäte
make test V=1    # s výpisom po taktoch
make alu         # samostatný test ALU
make uart        # samostatný test vysielača UART
```

Aktuálny výsledok: **26/26 testov prejde** v oboch režimoch.

### Test UART

Súbor: `rtl/uart/tb_uart_tx.cpp`

Vysielač sa testuje samostatne, s `CLKS_PER_BIT` = 16, aby bola simulácia krátka. Pre sadu bajtov testbench bajt odošle,
vzorkuje linku `tx` v strede každého bitu a kontroluje štart bit (0), 8 dátových bitov (od najnižšieho) a stop bit (1),
a tiež že `busy` je počas celého rámca vysoko a potom opäť nízko.

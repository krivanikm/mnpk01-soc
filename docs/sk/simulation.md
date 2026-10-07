---
title: Simulácia
lang: sk
---

# Simulácia a testovanie

Návrh sa overuje simulátorom **Verilator**, testbenche sú napísané v C++.

## Test ALU

Súbor: `rtl/alu/sim_main.cpp`

Vyskúša **všetky kombinácie** vstupov: každú operáciu so všetkými hodnotami `a` a `b` (256 × 256)
a porovná výsledok aj flagy s očakávanou hodnotou vypočítanou v C++.

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

Každý test sa spustí v **oboch režimoch pamäte**:

| Režim | Správanie |
|---|---|
| kombinačná | inštrukcia je k dispozícii okamžite |
| synchrónna (BRAM) | ako skutočná bloková pamäť vo FPGA: dáta prídu takt po adrese |

Program counter a pamäť programu zatiaľ emuluje testbench.

### Testy

| Test | Čo overuje |
|---|---|
| `basic` | MVI, MVIB, MOV, NOP spolu |
| `mvi_mvib` | 16-bitové konštanty, registre R0 a R15, prepísanie hodnoty |
| `mov` | MOV kopíruje iba dolný bajt, reťazenie, MOV sám na seba |
| `nop_unimplemented` | NOP a zatiaľ neimplementované inštrukcie procesor nezaseknú |
| `alu_add` | 59 + 49 = 108 |
| `alu_ops` | SUB, AND, OR, XOR, zmena operácie v R1 medzi inštrukciami |
| `alu_sub_borrow` | odčítanie pod nulu, odčítanie do nuly |
| `alu_cmp` | CMP nezapisuje do Rd |
| `alu_rd_eq_src` | cieľ je ten istý register ako zdroj |
| `alu_unary` | INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR |
| `alu_high_byte` | ALU mení iba dolný bajt |
| `alu_op_runtime` | ALU zapíše do R1 a tým vyberie ďalšiu operáciu |

### Spustenie

V priečinku `rtl/`:

```
make test        # všetky testy, oba režimy pamäte
make test V=1    # s výpisom po taktoch
```

Aktuálny výsledok: **12/12 testov prejde** v oboch režimoch.

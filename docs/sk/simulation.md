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
| `mov` | MOV kopíruje celých 16 bitov, reťazenie, MOV sám na seba |
| `nop_unimplemented` | NOP a zatiaľ neimplementované inštrukcie procesor nezaseknú |
| `alu_add` | 59 + 49 = 108 |
| `alu_ops` | SUB, AND, OR, XOR, zmena operácie v R1 medzi inštrukciami |
| `alu_sub_borrow` | odčítanie pod nulu, odčítanie do nuly |
| `alu_cmp` | CMP nezapisuje do Rd |
| `alu_rd_eq_src` | cieľ je ten istý register ako zdroj |
| `alu_unary` | INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR |
| `alu_high_byte` | ALU použije aj horné bajty operandov a prepíše celý Rd |
| `alu_carry16` | prenos / výpožička medzi dolným a horným bajtom, pretečenie 16 bitov |
| `alu_op_runtime` | ALU zapíše do R1 a tým vyberie ďalšiu operáciu |

### Spustenie

V priečinku `rtl/`:

```
make test        # všetky testy, oba režimy pamäte
make test V=1    # s výpisom po taktoch
make alu         # samostatný test ALU
```

Aktuálny výsledok: **13/13 testov prejde** v oboch režimoch.

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

Súbor: `rtl/tb_full_system.cpp`

Spustí krátky program a kontroluje, či sa do registrov zapísali správne hodnoty v správnom poradí.
Program counter a pamäť programu zatiaľ emuluje samotný testbench.

```
0x12AA   MVI  R2, 0xAA
0x3255   MVIB R2, 0x55
0x2520   MOV  R5, R2
0x110F   MVI  R1, 0x0F
0x0000   NOP
0x1333   MVI  R3, 0x33
```

Preklad a spustenie (v priečinku `rtl/`):

```
verilator --cc control-unit/controlunit.sv registers/register_file.sv system_top.sv \
    --exe tb_full_system.cpp --build --top system_top \
    -Wno-EOFNEWLINE -Wno-UNUSEDSIGNAL -Wno-UNDRIVEN -Wno-DECLFILENAME
./obj_dir/Vsystem_top          # kombinačná pamäť programu
./obj_dir/Vsystem_top sync     # synchrónna pamäť (ako BRAM vo FPGA)
```

| Režim | Výsledok |
|---|---|
| kombinačná pamäť | všetky testy prejdú |
| synchrónna pamäť (BRAM) | zatiaľ nefunguje – control unit potrebuje čakací stav pri načítaní inštrukcie |

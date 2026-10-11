---
title: Home
---

# MNPK-01

**MNPK-01** is a 16-bit microcomputer designed from the logic level in SystemVerilog all the way to a physical printed circuit board.
The goal of the project is to cover the whole chain of computer design: a custom instruction set architecture (ISA), its hardware
implementation, verification by simulation, deployment on an FPGA and finally a custom PCB.

## Key parameters

| Parameter | Value |
|---|---|
| Data width | 16 bits (registers, ALU) |
| Address width | 16 bits |
| Instruction width | 16 bits (1 instruction = 1 word) |
| Registers | 16 × 16 bits (R0 – R15) |
| Data memory | 4096 × 16 bits RAM (FPGA block RAM), word-addressed |
| Architecture | Harvard – separate program and data memory |
| Program storage | FPGA BRAM (planned: loaded from external SPI flash at boot) |

## Project status

| Part | Status |
|---|---|
| ALU | done, 16-bit, 14 operations, ~40 million checks (edge and random values) |
| Register file | done |
| Program counter | done, connected in `system_top` |
| Control unit | all 8 instructions work (NOP, MVI, MOV, MVIB, LOAD, STORE, JMP, ALU), also with FPGA BRAM |
| ALU instruction (via register R1) | done |
| Jumps (JMP + 6 conditions) | done – loops and decisions work |
| LOAD/STORE + data RAM | done – arrays in memory work |
| Automated tests | 19/19 pass in both memory modes |
| I/O (LEDs, UART), new instructions, assembler | planned |
| Synthesis (Yosys) | design fits a Tang Nano 9K with ~14 % of its LUTs and 4 of 26 block RAMs, see [Schematics](schematics.html) |
| FPGA, PCB | planned |

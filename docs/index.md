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
| Architecture | Harvard – separate program and data memory |
| Program storage | FPGA BRAM, loaded from external SPI flash at boot |

## Project status

| Part | Status |
|---|---|
| ALU | done, 16-bit, ~40 million tests (edge and random values) |
| Register file | done |
| Program counter | done (not yet connected in `system_top`) |
| Control unit | in progress – NOP, MVI, MVIB, MOV and ALU work (16-bit), also with FPGA BRAM |
| ALU instruction (via register R1) | done, 13 automated tests pass |
| Jumps, LOAD/STORE, RAM, I/O | being designed |
| FPGA, PCB | planned |

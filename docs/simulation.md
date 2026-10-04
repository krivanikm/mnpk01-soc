---
title: Simulation
---

# Simulation and testing

The design is verified with the **Verilator** simulator, testbenches are written in C++.

## ALU test

File: `rtl/alu/sim_main.cpp`

Tries **every combination** of inputs: each operation with all values of `a` and `b` (256 × 256),
and compares both the result and the flags with the expected values computed in C++.

## Full system test

File: `rtl/tb_full_system.cpp`

Runs a short program and checks that the correct values were written to the registers in the correct order.
The program counter and program memory are emulated by the testbench for now.

```
0x12AA   MVI  R2, 0xAA
0x3255   MVIB R2, 0x55
0x2520   MOV  R5, R2
0x110F   MVI  R1, 0x0F
0x0000   NOP
0x1333   MVI  R3, 0x33
```

Build and run (in the `rtl/` directory):

```
verilator --cc control-unit/controlunit.sv registers/register_file.sv system_top.sv \
    --exe tb_full_system.cpp --build --top system_top \
    -Wno-EOFNEWLINE -Wno-UNUSEDSIGNAL -Wno-UNDRIVEN -Wno-DECLFILENAME
./obj_dir/Vsystem_top          # combinational program memory
./obj_dir/Vsystem_top sync     # synchronous memory (like FPGA BRAM)
```

| Mode | Result |
|---|---|
| combinational memory | all tests pass |
| synchronous memory (BRAM) | does not work yet – the control unit needs a wait state when fetching an instruction |

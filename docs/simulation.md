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

Files: `rtl/tb_full_system.cpp`, `rtl/tests/*.txt`

Every test is a short program in its own text file together with the expected result:

```
1100    ; MVI R1, 0      (ADD)
123B    ; MVI R2, 59
1331    ; MVI R3, 49
7234    ; ALU R2, R3, R4
expect r2=59 r3=49 r4=108
```

The testbench loads the program, runs it and at the end compares the **actual contents of all 16 registers**
with the expected values. A register that is not listed must be 0, so a write to a wrong register is caught too.
It also checks the final PC and stops a program that gets stuck.

Every test runs in **both memory modes**:

| Mode | Behaviour |
|---|---|
| combinational | the instruction is available immediately |
| synchronous (BRAM) | like real FPGA block RAM: data arrive one cycle after the address |

The program counter and program memory are still emulated by the testbench for now.

### Tests

| Test | What it checks |
|---|---|
| `basic` | MVI, MVIB, MOV, NOP together |
| `mvi_mvib` | 16-bit constants, registers R0 and R15, overwriting a value |
| `mov` | MOV copies only the low byte, chaining, MOV to itself |
| `nop_unimplemented` | NOP and not yet implemented instructions do not hang the CPU |
| `alu_add` | 59 + 49 = 108 |
| `alu_ops` | SUB, AND, OR, XOR, operation in R1 changed between instructions |
| `alu_sub_borrow` | subtraction below zero, subtraction to zero |
| `alu_cmp` | CMP does not write to Rd |
| `alu_rd_eq_src` | destination equal to a source register |
| `alu_unary` | INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR |
| `alu_high_byte` | ALU changes only the low byte |
| `alu_op_runtime` | the ALU writes into R1 and so selects the next operation |

### Running

In the `rtl/` directory:

```
make test        # all tests, both memory modes
make test V=1    # with a cycle-by-cycle trace
```

Current result: **12/12 tests pass** in both modes.

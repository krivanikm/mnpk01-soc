---
title: Simulation
---

# Simulation and testing

The design is verified with the **Verilator** simulator, testbenches are written in C++.

## ALU test

File: `rtl/alu/sim_main.cpp`

With 16-bit operands there are too many combinations to try them all (65536 × 65536 per operation), so for every operation it tests:

1. all 65536 values of `a` against edge values of `b` (`0`, `1`, `0x7F`, `0x80`, `0xFF`, `0x100`, `0x7FFF`, `0x8000`, `0xFFFF`, …),
2. edge values of `a` against all 65536 values of `b`,
3. one million random pairs (with a fixed seed, so the test is always the same).

That is about **40 million checks**; both the result and the flags are compared with the values computed in C++.
Run it with `make alu` in `rtl/`.

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

The program counter is real hardware (`programcounter.v` in `system_top`); the testbench only emulates the program memory and serves it the instruction at address `pc`.

### Tests

| Test | What it checks |
|---|---|
| `basic` | MVI, MVIB, MOV, NOP together |
| `mvi_mvib` | 16-bit constants, registers R0 and R15, overwriting a value |
| `mov` | MOV copies all 16 bits, chaining, MOV to itself |
| `nop_unimplemented` | NOP and not yet implemented instructions (LOAD, STORE) do not hang the CPU |
| `alu_add` | 59 + 49 = 108 |
| `alu_ops` | SUB, AND, OR, XOR, operation in R1 changed between instructions |
| `alu_sub_borrow` | subtraction below zero, subtraction to zero |
| `alu_cmp` | CMP does not write to Rd |
| `alu_rd_eq_src` | destination equal to a source register |
| `alu_unary` | INC, DEC, NOT, SHL, SHR, ROL, PASS, CLR |
| `alu_high_byte` | the ALU uses the high bytes of the operands and overwrites the whole Rd |
| `alu_carry16` | carry / borrow between the low and high byte, 16-bit overflow |
| `alu_op_runtime` | the ALU writes into R1 and so selects the next operation |
| `jmp_always` | unconditional jump forwards and backwards, skipped instructions are not executed |
| `jmp_cond` | all 6 conditions after CMP, each one taken and not taken; an unknown condition never jumps |
| `jmp_loop` | a loop: 6 × 7 = 42 by repeated addition, JNZ on the Z flag from SUB |

### Running

In the `rtl/` directory:

```
make test        # all tests, both memory modes
make test V=1    # with a cycle-by-cycle trace
make alu         # standalone ALU test
```

Current result: **16/16 tests pass** in both modes.

---
title: Architecture
---

# Architecture

```
              +-----------------+
   SPI flash  |  Program memory | 16-bit instruction
  ----------> |     (BRAM)      | ------------------+
              +-----------------+                   |
                       ^ 16-bit address             v
              +-----------------+          +-----------------+
              | Program counter | <------- |  Control unit   |
              +-----------------+   jump   +-----------------+
                                             |      |      ^
                                   control   |      |      | flags Z/C/N
                                             v      v      |
                                  +---------------+  +-----------+
                                  | Register file |<>|    ALU    |
                                  |  16 x 16 bit  |  |   8 bit   |
                                  +---------------+  +-----------+
```

The control unit drives all blocks directly, there is no bus-management unit between them.
The ALU takes its operation code straight from register R1 (see [ALU](alu.html)).

## Program counter

File: `rtl/program-counter/programcounter.v`

A 16-bit register holding the address of the current instruction.

| Signal | Direction | Meaning |
|---|---|---|
| `clk`, `rst` | input | clock, asynchronous reset (PC = 0) |
| `pc_inc` | input | PC = PC + 1 |
| `pc_load` | input | PC = `d_in` (jump) |
| `d_in[15:0]` | input | jump target address |
| `pc[15:0]` | output | current address |

## Register file

File: `rtl/registers/register_file.sv`

16 registers of 16 bits each. The data path is 8 bits wide, so registers are written and read
**byte by byte** – the `high_b` signal selects the high (`[15:8]`) or low (`[7:0]`) byte.

| Signal | Direction | Meaning |
|---|---|---|
| `clk`, `rst_n` | input | clock, asynchronous active-low reset, clears all registers |
| `addr[3:0]` | input | register number R0 – R15 |
| `high_b` | input | 1 = high byte, 0 = low byte |
| `d_in[7:0]` | input | byte to write |
| `write_en` | input | write on the rising clock edge |
| `q_out[7:0]` | output | byte read (combinational, valid right after the address changes) |
| `r1_out[3:0]` | output | `R1[3:0]`, wired straight to the ALU as the operation code |

## Control unit

File: `rtl/control-unit/controlunit.sv`

A finite state machine (FSM) that walks through these states for every instruction:

```
S_FETCH -> S_DECODE -> execute states -> S_FETCH
```

| State | What happens |
|---|---|
| `S_FETCH` | instruction is read from program memory |
| `S_DECODE` | opcode is decoded, next state is chosen |
| `S_NOP` | empty cycle while the PC advances |
| `S_MVI_WRITE` | constant is written to a register (both MVI and MVIB) |
| `S_MOV_LATCH` | source register is read into a temporary register |
| `S_MOV_WRITE` | value is written to the destination register |
| `S_ALU_A` | operand Ra is stored, Rb is selected |
| `S_ALU_B` | ALU result is written to Rd (except CMP), flags are stored |

### Timing rule

The program memory in the FPGA (BRAM) is synchronous: data for a new address arrive one cycle later.
`S_FETCH` therefore stores the whole instruction in the instruction register and immediately sets `pc_inc`;
the following states only use the instruction register, so the memory fetches the next instruction in the meantime:

| Cycle | State | |
|---|---|---|
| 1 | `S_FETCH` | instruction → instruction register, `pc_inc` = 1 |
| 2 | `S_DECODE` | PC advances at the end of the cycle |
| 3 | execute | the memory latches the new address at the end of the cycle |
| 4 | `S_FETCH` | the next instruction is valid |

Rule: `pc_inc` is set **only** in `S_FETCH` and every instruction takes **at least 3 cycles**
(that is why NOP goes through `S_NOP`).

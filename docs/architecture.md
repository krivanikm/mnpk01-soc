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
              +-----------------+          +-----------------+  address, data  +-----------------+
              | Program counter | <------- |  Control unit   | <-------------> |    Data RAM     |
              +-----------------+   jump   +-----------------+   LOAD/STORE    | 4K x 16 (BRAM)  |
                                             |      |      ^                   +-----------------+
                                   control   |      |      | flags Z/C/N
                                             v      v      |
                                  +---------------+  +-----------+
                                  | Register file |<>|    ALU    |
                                  |  16 x 16 bit  |  |  16 bit   |
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

If `pc_inc` and `pc_load` are both 1, `pc_inc` wins – the jump is not taken.
The control unit never sets both at once: `pc_inc` only in `S_FETCH`, `pc_load` only in `S_JMP`.
The program counter is part of `system_top`; its output `pc` is the address for the program memory.

## Register file

File: `rtl/registers/register_file.sv`

16 registers of 16 bits each. A register is always **read as a whole 16-bit word**.
Writing has two enable bits, one per byte, so the control unit can write the low byte (MVI),
the high byte (MVIB) or the whole word (MOV, ALU).

| Signal | Direction | Meaning |
|---|---|---|
| `clk`, `rst_n` | input | clock, asynchronous active-low reset, clears all registers |
| `addr[3:0]` | input | register number R0 – R15 |
| `d_in[15:0]` | input | data to write |
| `write_en[1:0]` | input | write on the rising clock edge: `01` = low byte `[7:0]`, `10` = high byte `[15:8]`, `11` = whole word |
| `q_out[15:0]` | output | register read (combinational, valid right after the address changes) |
| `r1_out[3:0]` | output | `R1[3:0]`, wired straight to the ALU as the operation code |

## Data RAM

File: `rtl/memory/data_ram.sv`

4096 words of 16 bits, implemented in FPGA block RAM (4 BSRAM blocks on the Tang Nano 9K).
It is **synchronous**: both write and read happen on the rising clock edge, so read data are valid one cycle after the address.
The memory array has no reset on purpose – with a reset the synthesizer could not map it to block RAM and would use
65 536 flip-flops instead.

| Signal | Direction | Meaning |
|---|---|---|
| `clk` | input | clock |
| `addr[11:0]` | input | word address (the low 12 bits of the address from the register) |
| `din[15:0]` | input | data to write |
| `we` | input | 1 = write `din` to `addr` on the rising edge |
| `dout[15:0]` | output | data read from `addr`, valid one cycle later |

The upper bits of the address `[15:12]` are not used yet; they are reserved for memory-mapped I/O (see [Memory](isa.html#memory)).

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
| `S_JMP` | target register is read; if the condition holds: `pc_addr` = target, `pc_load` = 1 |
| `S_JMP_WAIT1` | the PC takes the target address at the end of the cycle |
| `S_JMP_WAIT2` | the memory latches the target address at the end of the cycle |
| `S_STORE_ADDR` | address register Ra is read into `mem_addr`, data register Rs is selected |
| `S_STORE_DATA` | Rs goes to `mem_wdata`, `mem_we` = 1 – the RAM writes at the end of the next cycle |
| `S_LOAD_ADDR` | address register Ra is read into `mem_addr` |
| `S_LOAD_WAIT` | the RAM latches the address at the end of the cycle |
| `S_LOAD_WRITE` | data from the RAM are written to Rd |

The state register has 5 bits (16 states are used, so the next one would not fit into 4 bits).

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

### Jump timing

The outputs of the control unit are registered and the memory is synchronous, so a taken jump needs two extra cycles
before the instruction at the target address T can be fetched:

| Cycle | State | What happens | PC | Memory latches |
|---|---|---|---|---|
| 1 | `S_FETCH` | JMP → instruction register, `pc_inc` = 1 | J | J |
| 2 | `S_DECODE` | register address ← Rr | J → J+1 | J |
| 3 | `S_JMP` | target is read, `pc_load` = 1 (if the condition holds) | J+1 | J+1 |
| 4 | `S_JMP_WAIT1` | PC takes the target at the end of the cycle | J+1 → T | J+1 |
| 5 | `S_JMP_WAIT2` | | T | **T** |
| 6 | `S_FETCH` | the instruction at T is valid | T | |

A jump that is **not taken** goes from `S_JMP` straight back to `S_FETCH` – the PC already points past the JMP,
so it takes 3 cycles like any other short instruction.

The condition is evaluated by the combinational signal `cond_ok` from `ir[11:8]` and the stored flags.

### LOAD / STORE timing

The register file has a single read port, so STORE reads its two registers (address and data) one after another.
LOAD has to wait one cycle for the synchronous RAM:

| Cycle | STORE | LOAD |
|---|---|---|
| 1 | `S_FETCH` | `S_FETCH` |
| 2 | `S_DECODE` – register address ← Ra | `S_DECODE` – register address ← Ra |
| 3 | `S_STORE_ADDR` – `mem_addr` ← Ra, register address ← Rs | `S_LOAD_ADDR` – `mem_addr` ← Ra |
| 4 | `S_STORE_DATA` – `mem_wdata` ← Rs, `mem_we` = 1 | `S_LOAD_WAIT` – the RAM latches the address |
| 5 | (next `S_FETCH`, the RAM writes at the end of the cycle) | `S_LOAD_WRITE` – Rd ← `mem_rdata` |

STORE takes **4 cycles**, LOAD **5 cycles**.

## Schematics

RTL schematics of every block generated by Yosys are on the [Schematics](schematics.html) page.

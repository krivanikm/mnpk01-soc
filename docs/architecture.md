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

The RAM only sees the low 12 bits of the address; the I/O area `0xFF00` – `0xFFFF` is decoded in `system_top` before the RAM (see [Input / output](#input--output)).

## Input / output

Input and output are **memory-mapped**: the same `LOAD` and `STORE` instructions that access the RAM also reach the ports, there is no separate I/O instruction.
`system_top` decodes the full 16-bit address of the memory access:

```
io_sel = (mem_addr[15:8] == 8'hFF)          the address is in 0xFF00 – 0xFFFF
RAM write enable = mem_we && !io_sel         a STORE to the I/O area never writes into the RAM
io_out <= mem_wdata   when mem_we && mem_addr == 16'hFF00
```

| Port | Direction | Address | Description |
|---|---|---|---|
| `io_out[15:0]` | output | `0xFF00` (STORE) | a 16-bit register, reset to 0 |
| `io_in[15:0]` | input | `0xFF01` (LOAD) | value of the pins, read through the multiplexer |
| UART data | output | `0xFF01` (STORE) | the low byte is sent over the `uart_tx_pin` pin; ignored while the UART is busy |
| UART status | input | `0xFF02` (LOAD) | bit 0 = `busy` (1 while a byte is being sent), the other bits are 0 |

`0xFF01` has a different meaning for each direction: `STORE` starts the UART, `LOAD` reads the input pins.
Any `LOAD` from the I/O area except `0xFF02` returns `io_in`, the address is not decoded more precisely.

**Read path.** The RAM is synchronous, so LOAD waits one cycle for the data (`S_LOAD_WAIT`). The input port has to answer with the same delay:
`system_top` registers both `io_sel` and `io_in` in the cycle in which the RAM latches the address and then selects between the RAM output and the port:

```
w_mem_rdata = io_sel_q ? (io_status_q ? {15'b0, uart_busy} : io_in_q) : ram_dout
```

`io_status_q` is the registered result of the comparison `mem_addr == 0xFF02`, so the UART status also arrives with the same delay as RAM data.
Without this the CU would read the previous value of the port instead of the current one.
The tests `io_read` and `io_read_ram_mix` check exactly that, including alternating LOADs from the port and from the RAM.

### UART transmitter

File: `rtl/uart/uart_tx.sv`

A transmitter for the 8N1 format (1 start bit, 8 data bits starting with the least significant one, 1 stop bit, no parity).
The parameter `CLKS_PER_BIT` is the number of clocks per bit: 27 MHz / 115 200 Bd = 234 (the default).

| Signal | Direction | Meaning |
|---|---|---|
| `clk`, `rst` | input | clock, synchronous reset (line idle at 1) |
| `data[7:0]` | input | byte to send, latched when `start` = 1 |
| `start` | input | 1 for one clock in `IDLE` starts the transmission |
| `tx` | output | serial line, idle level 1 |
| `busy` | output | 1 in every state except `IDLE` |

The FSM has four states: `IDLE -> START -> DATA -> STOP -> IDLE`, a counter `clk_cnt` counts the clocks inside one bit and `bit_idx` the data bit.
One byte takes 10 bits, i.e. about 87 µs at 115 200 Bd, which is thousands of CPU cycles.

In `system_top`: `uart_start = mem_we && mem_addr == 0xFF01 && !uart_busy`, the data are `mem_wdata[7:0]`.
The UART has no buffer, so a program has to wait for `busy` = 0 before every byte, otherwise the byte is lost. Sending one character:

```
wait:  LOAD  R6, [R3]      ; R3 = 0xFF02, R6 = busy
       ...                 ; R6 != 0 -> jump back to wait
       STORE R5, [R2]      ; R2 = 0xFF01, send the low byte of R5
```

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

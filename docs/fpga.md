---
title: FPGA
---

# FPGA

The CPU runs on a **Tang Nano 9K** board (Gowin GW1NR-9, 27 MHz oscillator, 32 Mbit SPI flash, 6 LEDs). The same RTL that is verified in the simulator
is used unchanged: only a small top module around it is new.

## Build flow

The whole flow is open source:

| Step | Tool |
|---|---|
| synthesis | Yosys (`synth_gowin`) |
| place and route | nextpnr (himbaechel, Gowin) |
| bitstream | Apicula (`gowin_pack`) |
| programming | openFPGALoader |

In the `fpga/` directory:

```
make          # top.fs bitstream
make flash    # load it into the FPGA (SRAM)
```

The place and route report gives a maximum clock of about 40 MHz, the board clock is 27 MHz. The design uses 5 of 26 block RAMs.

## Top module

File: `fpga/top.sv`. It contains only the glue around `system_top`:

| Part | What it does |
|---|---|
| reset | a 4-bit counter holds `rst` high for 15 clocks after power-up (the board has no reset button) |
| program memory | a 256 × 16 array filled from `prog.hex` with `$readmemh`, addressed by `pc` |
| input synchronizer | two registers in a row for the switches, so the CPU never sees a signal that changes right at the clock edge |
| ports | the inverted switch value goes to the low byte of `io_in`, the low byte of `io_out` goes to the LEDs |

## Input / output on the board

| Address | Access | Hardware |
|---|---|---|
| `0xFF00` | `STORE` | `io_out[7:0]` to 8 external LEDs (each through a 330 Ω resistor) |
| `0xFF01` | `LOAD` | `io_in[7:0]` from an 8-position DIP switch (ON = 1, the pins use pull-ups) |
| `0xFF01` | `STORE` | the low byte is sent over the UART (pin 17, 115 200 Bd, 8N1) |
| `0xFF02` | `LOAD` | UART status, bit 0 = busy |

| Signal | Pins |
|---|---|
| clock | 52 |
| DIP switch bits 0 – 7 | 25, 26, 27, 28, 29, 30, 36, 37 |
| LED bits 0 – 7 | 33, 34, 40, 35, 41, 42, 51, 53 |
| `uart_tx` | 17 |

The pin assignment is in `fpga/tang9k.cst`.

The UART output goes through the USB-UART bridge on the board, so no extra hardware is needed: the text appears on the computer as a serial port
(`/dev/ttyUSB1` on Linux, e.g. `picocom -b 115200 /dev/ttyUSB1`).
The program in `fpga/prog.hex` sends the text `Hello World` and a newline in a loop, before every character it waits until `busy` = 0.

## Programs that ran on the board

Before the I/O port existed, the CPU was tested on the board with a wrapper that watched the writes into register R12 and used them as an output channel.
The CPU RTL stayed unchanged. There is no multiply or divide instruction, so both are done with shifts and additions.

| Program | Description |
|---|---|
| digits of π | spigot algorithm, the CPU computed 980 and later 1950 digits, shown on a 1.14" display |
| Mandelbrot set | 135 × 240 pixels, 16-bit fixed point (3.12), multiplication by shift and add; one frame takes about 7 seconds and the picture zooms into a chosen point |

These two programs and the display driver were made with the help of Claude, an AI assistant from Anthropic.

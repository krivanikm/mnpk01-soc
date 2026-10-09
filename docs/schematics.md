---
title: Schematics
---

# Schematics

RTL schematics generated from the SystemVerilog code by **Yosys** (`make schematics` in `rtl/`).
Every block is drawn the way the synthesizer understands it: adders, multiplexers, registers and the wires between them.
Each schematic is an SVG, so it stays sharp at any zoom.

<p class="sv-hint">drag to move · + / − or Ctrl + wheel to zoom · double-click to zoom in · ⛶ fullscreen (the wheel zooms there)</p>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/system_top.svg' | relative_url }}" data-alt="system_top schematic"></div>
  <figcaption><b>system_top</b> – how the control unit, register file, ALU and program counter are connected. Note <code>r1_out</code> going straight into the <code>op</code> input of the ALU.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/programcounter.svg' | relative_url }}" data-alt="program counter schematic"></div>
  <figcaption><b>Program counter</b> – a +1 adder, two multiplexers and a 16-bit register. The <code>pc_inc</code> multiplexer is closer to the register, so it has priority over <code>pc_load</code>.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/alu.svg' | relative_url }}" data-alt="ALU schematic"></div>
  <figcaption><b>ALU</b> – the arithmetic and logic operations computed in parallel and a large multiplexer that picks one result by <code>op</code>.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/controlunit.svg' | relative_url }}" data-alt="control unit schematic"></div>
  <figcaption><b>Control unit</b> – the state machine, the instruction register and the flags.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/register_file.svg' | relative_url }}" data-alt="register file schematic"></div>
  <figcaption><b>Register file</b> – 16 × 16-bit registers and the multiplexer that selects the register being read. This is the largest block of the CPU.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/cpu_full.svg' | relative_url }}" data-alt="whole CPU schematic"></div>
  <figcaption><b>Whole CPU</b> – everything above flattened into a single diagram. Very large – use fullscreen and zoom in.</figcaption>
</figure>

## FPGA resources

Synthesis for the Gowin GW1NR-9 (Tang Nano 9K) with Yosys `synth_gowin`:

| Resource | CPU | Tang Nano 9K | Usage |
|---|---|---|---|
| Flip-flops | 367 | 6 480 | ~6 % |
| LUTs | 1 405 | 8 640 | ~16 % |

Most of it is the register file: 256 bits of state and a 16-to-1 multiplexer 16 bits wide.
Most of the ~920 single-input LUTs are only constant helper inputs for the wide multiplexers that Yosys adds for Gowin; the real logic (LUT2 – LUT4) is about 480 LUTs.

<script src="{{ '/assets/js/schem.js' | relative_url }}?v=1.3"></script>

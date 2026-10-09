---
title: Schémy
lang: sk
---

# Schémy

RTL schémy vygenerované zo SystemVerilog kódu nástrojom **Yosys** (`make schematics` v `rtl/`).
Každý blok je nakreslený tak, ako ho chápe syntéza: sčítačky, multiplexery, registre a vodiče medzi nimi.
Každá schéma je SVG, takže ostane ostrá pri akomkoľvek priblížení.

<p class="sv-hint">ťahaj = posun · + / − alebo Ctrl + koliesko = zoom · dvojklik = priblíženie · ⛶ celá obrazovka (tam zoomuje aj koliesko)</p>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/system_top.svg' | relative_url }}" data-alt="schéma system_top"></div>
  <figcaption><b>system_top</b> – ako sú prepojené control unit, register file, ALU a program counter. Všimni si <code>r1_out</code>, ktorý ide priamo do vstupu <code>op</code> ALU.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/programcounter.svg' | relative_url }}" data-alt="schéma program countera"></div>
  <figcaption><b>Program counter</b> – sčítačka +1, dva multiplexery a 16-bitový register. Multiplexer pre <code>pc_inc</code> je bližšie k registru, preto má prednosť pred <code>pc_load</code>.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/alu.svg' | relative_url }}" data-alt="schéma ALU"></div>
  <figcaption><b>ALU</b> – aritmetické a logické operácie sa počítajú naraz a veľký multiplexer podľa <code>op</code> vyberie jeden výsledok.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/controlunit.svg' | relative_url }}" data-alt="schéma control unit"></div>
  <figcaption><b>Control unit</b> – konečný automat, inštrukčný register a flagy.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/register_file.svg' | relative_url }}" data-alt="schéma register file"></div>
  <figcaption><b>Register file</b> – 16 registrov po 16 bitoch a multiplexer, ktorý vyberá čítaný register. Je to najväčší blok procesora.</figcaption>
</figure>

<figure class="schem">
  <div class="sv" data-src="{{ '/assets/schematics/cpu_full.svg' | relative_url }}" data-alt="schéma celého procesora"></div>
  <figcaption><b>Celý procesor</b> – všetko vyššie spojené do jednej schémy. Je veľmi veľká – daj ju na celú obrazovku a priblíž si ju.</figcaption>
</figure>

## Zdroje FPGA

Syntéza pre Gowin GW1NR-9 (Tang Nano 9K) nástrojom Yosys `synth_gowin`:

| Zdroj | Procesor | Tang Nano 9K | Využitie |
|---|---|---|---|
| Klopné obvody | 367 | 6 480 | ~6 % |
| LUT | 1 405 | 8 640 | ~16 % |

Väčšinu zaberá register file: 256 bitov stavu a 16-bitový multiplexer 1 zo 16.
Väčšina z ~920 jednovstupových LUT sú iba konštantné pomocné vstupy pre široké multiplexery, ktoré Yosys pre Gowin pridáva; skutočnej logiky (LUT2 – LUT4) je asi 480 LUT.

<script src="{{ '/assets/js/schem.js' | relative_url }}?v=1.3"></script>

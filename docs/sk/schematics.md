---
title: Schémy
lang: sk
---

# Schémy

RTL schémy vygenerované zo SystemVerilog kódu nástrojom **Yosys** (`make schematics` v `rtl/`).
Každý blok je nakreslený tak, ako ho chápe syntéza: sčítačky, multiplexery, registre a vodiče medzi nimi.
**Klikni na schému** a otvorí sa v plnej veľkosti – je to SVG, takže ostane ostrá pri akomkoľvek priblížení.

<figure class="schem">
  <a href="{{ '/assets/schematics/system_top.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/system_top.svg' | relative_url }}" alt="schéma system_top" loading="lazy"></a>
  <figcaption><b>system_top</b> – ako sú prepojené control unit, register file a ALU. Všimni si <code>r1_out</code>, ktorý ide priamo do vstupu <code>op</code> ALU.</figcaption>
</figure>

<figure class="schem">
  <a href="{{ '/assets/schematics/programcounter.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/programcounter.svg' | relative_url }}" alt="schéma program countera" loading="lazy"></a>
  <figcaption><b>Program counter</b> – sčítačka +1, dva multiplexery a 16-bitový register. Multiplexer pre <code>pc_inc</code> je bližšie k registru, preto má prednosť pred <code>pc_load</code>.</figcaption>
</figure>

<figure class="schem">
  <a href="{{ '/assets/schematics/alu.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/alu.svg' | relative_url }}" alt="schéma ALU" loading="lazy"></a>
  <figcaption><b>ALU</b> – aritmetické a logické operácie sa počítajú naraz a veľký multiplexer podľa <code>op</code> vyberie jeden výsledok.</figcaption>
</figure>

<figure class="schem">
  <a href="{{ '/assets/schematics/controlunit.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/controlunit.svg' | relative_url }}" alt="schéma control unit" loading="lazy"></a>
  <figcaption><b>Control unit</b> – konečný automat, inštrukčný register a flagy.</figcaption>
</figure>

<figure class="schem">
  <a href="{{ '/assets/schematics/register_file.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/register_file.svg' | relative_url }}" alt="schéma register file" loading="lazy"></a>
  <figcaption><b>Register file</b> – 16 registrov po 16 bitoch a multiplexer, ktorý vyberá čítaný register. Je to najväčší blok procesora.</figcaption>
</figure>

<figure class="schem">
  <a href="{{ '/assets/schematics/cpu_full.svg' | relative_url }}" target="_blank" rel="noopener"><img src="{{ '/assets/schematics/cpu_full.svg' | relative_url }}" alt="schéma celého procesora" loading="lazy"></a>
  <figcaption><b>Celý procesor</b> – všetko vyššie spojené do jednej schémy. Je veľmi veľká – otvor ju a priblíž si ju.</figcaption>
</figure>

## Zdroje FPGA

Syntéza pre Gowin GW1NR-9 (Tang Nano 9K) nástrojom Yosys `synth_gowin`:

| Zdroj | Procesor | Tang Nano 9K | Využitie |
|---|---|---|---|
| Klopné obvody | 347 | 6 480 | ~5 % |
| LUT | 1 296 | 8 640 | ~15 % |

Väčšinu zaberá register file: 256 bitov stavu a 16-bitový multiplexer 1 zo 16.
Z počtu LUT je asi 730 iba konštantných pomocných vstupov pre široké multiplexery, ktoré Yosys pre Gowin pridáva; skutočnej logiky je asi 330 LUT.

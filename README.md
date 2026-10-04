# MNPK01-System: Full-Stack 8-bit Microcomputer

![Status](https://img.shields.io/badge/Status-Development-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-GPLv3-green?style=for-the-badge)

📖 **Documentation:** [krivanikm.github.io/mnpk01-soc](https://krivanikm.github.io/mnpk01-soc/) (English / Slovenčina)

## 📌 Project Overview
**MNPK01-System** is a 8-bit microcomputer architecture, engineered from the silicon level up to the physical PCB. Developed as a comprehensive graduation thesis, this project bridges the gap between Hardware Description Languages (HDL) and physical hardware implementation.

The core mission of the MNPK01 is to demonstrate a **complete vertical integration** of computer systems: from logical gate synthesis and custom Instruction Set Architecture (ISA) to physical PCB fabrication and low-level firmware optimization.

---

## 🏗 System Architecture

The MNPK-01 uses a Harvard architecture with an 8-bit data path and 16-bit addressing. The **Control Unit** drives every block directly – the register file, the ALU, data memory and I/O – with no separate bus-management unit in between. The ALU is used in a syscall-like way: the operation code is stored in register **R1** and a single `ALU Ra, Rb, Rd` instruction executes it.

### Integrated System Map
```mermaid
graph TD
    %% Global Styling
    classDef control fill:#ffd166,stroke:#000,stroke-width:2px,color:#000;
    classDef storage fill:#06d6a0,stroke:#000,stroke-width:2px,color:#000;
    classDef compute fill:#ef476f,stroke:#000,stroke-width:2px,color:#fff;
    classDef routing fill:#118ab2,stroke:#000,stroke-width:2px,color:#fff;

    subgraph Boot_Configuration [Boot Storage]
        FLASH[SPI Flash - External] -.->|Boot / Load| BRAM[FPGA Memory / BRAM]
    end

    subgraph Instruction_Fetch [Instruction Fetch Unit]
        PC[Program Counter] -->|16-bit Address| BRAM
    end

    BRAM -->|16-bit Instruction| CU[CONTROL UNIT]

    subgraph Execution_Core [Processing Core]
        REGS[REGISTER FILE]
        ALU[ALU - Arithmetic Logic Unit]

        REGS -->|Operands Ra, Rb| ALU
        REGS -.->|R1: ALU Opcode| ALU
        ALU -->|Result to Rd| REGS
    end

    %% Control Signals
    CU -->|Jump Logic| PC
    CU -.->|Write Enable| REGS
    ALU -.->|Flags: Z/C| CU

    %% Peripherals
    REGS <-->|Memory Interface| RAM[RAM - Data Memory]
    REGS <-->|I/O Interface| IO[I/O PORTS]
    CU -.->|Read / Write| RAM
    CU -.->|Read / Write| IO

    %% Applying Styles
    class CU control;
    class FLASH,BRAM,RAM,REGS storage;
    class ALU compute;
    class PC routing;

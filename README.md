# MNPK01-System: Full-Stack 8-bit Microcomputer

![Status](https://img.shields.io/badge/Status-Development-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-GPLv3-green?style=for-the-badge)

## 📌 Project Overview
**MNPK01-System** is a 8-bit microcomputer architecture, engineered from the silicon level up to the physical PCB. Developed as a comprehensive graduation thesis, this project bridges the gap between Hardware Description Languages (HDL) and physical hardware implementation.

The core mission of the MNPK01 is to demonstrate a **complete vertical integration** of computer systems: from logical gate synthesis and custom Instruction Set Architecture (ISA) to physical PCB fabrication and low-level firmware optimization.

---

## 🏗 System Architecture

The heart of the MNPK-01 is a modular **Resource Handler-centric** design. By utilizing a centralized bus management system, the architecture eliminates data contention and provides a streamlined path for 8-bit operations with 16-bit addressing capabilities.

### Integrated System Map

```mermaid
graph TD
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
        ALU[ALU]
    end

    CU -->|addr / write_en / data| REGS
    REGS -->|q_out| CU
    CU -->|func code via fixed registers| ALU
    ALU -->|Flags: Z/C/N| CU

    CU -->|Jump addr / pc_inc| PC

    CU <-->|Memory access sequence| RAM[RAM - Data Memory]
    CU <-->|IO sequence| IO[I/O PORTS]

    class CU control;
    class FLASH,BRAM,RAM,REGS storage;
    class ALU compute;
    class PC routing;
```

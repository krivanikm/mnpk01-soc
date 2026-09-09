#include <verilated.h>
#include "Vsystem_top.h"
#include <iostream>
#include <iomanip>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Vsystem_top* top = new Vsystem_top;
    int sim_time = 0;

    top->clk = 0;
    top->rst = 1;
    top->rom_data = 0x0000;

    std::cout << "--- START FULL SYSTEM TEST (MVI) ---" << std::endl;

    while (sim_time < 90) {
        if (sim_time == 10) {
            top->rst = 0;
            // Nastavíme inštrukciu MVI hneď po uvoľnení resetu
            // Opcode (0x01) na [15:8] -> 0x01...
            // RegAddr (2) na [15:12] ? Pozor: rom_data[15:8] sa berie v S_FETCH, 
            // ale v S_COLLECT sa berie rom_data[15:12] pre reg_addr a rom_data[11:4] pre dáta (0xAA).
            // Skúsme celkovo rom_data = 0x012A (kde 0x01 je opcode, 2 je reg, AA sú dáta... ale 0x01 pretína [15:8]).
            // Pre istotu nastavíme statickú ROM vzorku: 
            top->rom_data = 0x012A; 
            std::cout << ">>> Reset uvoľnený, ROM data nastavené na 0x012A" << std::endl;
        }

        // Vzostupná hrana hodín
        if (sim_time % 10 == 5) {
            top->clk = 1;
            top->eval();

            if (sim_time > 10) {
                std::cout << "Time=" << std::setw(2) << sim_time 
                          << " | State=" << (int)top->state_out 
                          << " | RegAddr=" << (int)top->reg_addr 
                          << " | DataIn=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)top->reg_data 
                          << " | RegQ=0x" << (int)top->reg_q_out
                          << " | HighB=" << std::dec << (int)top->high_b 
                          << " | WriteEn=" << (int)top->write_reg_en << std::endl;
            }
        }

        // Zostupná hrana hodín
        if (sim_time % 10 == 0) {
            top->clk = 0;
            top->eval();
        }

        sim_time++;
    }

    std::cout << "\n--- SIMULÁCIA ÚSPEŠNE DOKONČENÁ ---" << std::endl;

    delete top;
    return 0;
}
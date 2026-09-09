#include <verilated.h>
#include "Vregister_file.h"
#include <iostream>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vregister_file* rf = new Vregister_file;

    int sim_time = 0;

    // Inicializácia vstupov
    rf->clk = 0;
    rf->rst_n = 0; // Aktívny reset (negedge rst_n)
    rf->addr = 0;
    rf->high_b = 0;
    rf->d_in = 0;
    rf->write_en = 0;

    std::cout << "--- START REGISTER FILE TEST ---" << std::endl;

    while (sim_time < 50) {
        // Uvoľnenie resetu
        if (sim_time == 10) {
            rf->rst_n = 1;
            std::cout << ">>> Reset uvoľnený" << std::endl;
        }

        // Hodiny (taktovanie)
        if (sim_time % 10 == 5) {
            rf->clk = 1;
            rf->eval();
        }
        if (sim_time % 10 == 0) {
            rf->clk = 0;
            rf->eval();

            // 1. KROK: Zápis dolného bajtu (MVI simulácia) do R1 (addr = 1) s dátami 0xAA
            if (sim_time == 20) {
                rf->addr = 1;
                rf->high_b = 0;
                rf->d_in = 0xAA;
                rf->write_en = 1;
                std::cout << "Zapisujem 0xAA do dolného bajtu R1 (high_b=0)" << std::endl;
            }
            // 2. KROK: Zápis horného bajtu (MVIB simulácia) do R1 (addr = 1) s dátami 0x55
            else if (sim_time == 30) {
                rf->addr = 1;
                rf->high_b = 1;
                rf->d_in = 0x55;
                rf->write_en = 1;
                std::cout << "Zapisujem 0x55 do horného bajtu R1 (high_b=1)" << std::endl;
            }
            else {
                rf->write_en = 0; // Vypnúť zápis po takte
            }
        }

        sim_time++;
    }

    // Finálna kontrola čítania z R1
    std::cout << "\n--- KONTROLA VÝSLEDKU V R1 ---" << std::endl;
    rf->addr = 1;

    // Čítanje dolného bajtu
    rf->high_b = 0;
    rf->eval();
    std::cout << "Prečítaný dolný bajt: 0x" << std::hex << (int)rf->q_out << " (Očakávané: aa)" << std::endl;

    // Čítanie horného bajtu
    rf->high_b = 1;
    rf->eval();
    std::cout << "Prečítaný horný bajt: 0x" << (int)rf->q_out << std::dec << " (Očakávané: 55)" << std::endl;

    delete rf;
    return 0;
}

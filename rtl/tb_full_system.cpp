#include <verilated.h>
#include "Vsystem_top.h"
#include <cstdio>
#include <cstring>
#include <vector>

// Testbench zatiaľ sám emuluje program counter + ROM, kým nie sú v system_top.
// PC sa zvýši na nábežnej hrane, keď je pc_inc = 1 (rovnako ako programcounter.v).
//
// Režimy ROM:
//   (bez argumentu)  kombinačná ROM: rom_data = rom[pc] okamžite
//   sync             synchrónna ROM (ako FPGA BRAM): rom_data sa obnoví až na hrane

struct Write { int addr; int high; int data; };

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    bool sync_rom = argc > 1 && std::strcmp(argv[argc - 1], "sync") == 0;

    // Formát: [15:12] opcode | [11:8] register | [7:0] dáta  (MOV: [11:8] cieľ, [7:4] zdroj)
    const uint16_t rom[] = {
        0x12AA,  // MVI  r2, 0xAA   -> r2[7:0]  = 0xAA
        0x3255,  // MVIB r2, 0x55   -> r2[15:8] = 0x55
        0x2520,  // MOV  r5, r2     -> r5[7:0]  = r2[7:0] = 0xAA
        0x110F,  // MVI  r1, 0x0F   -> r1[7:0]  = 0x0F
        0x0000,  // NOP              -> nič, len PC++
        0x1333,  // MVI  r3, 0x33   -> r3[7:0]  = 0x33  (overí, že NOP posunul PC)
    };
    const int rom_size = sizeof(rom) / sizeof(rom[0]);
    const std::vector<Write> expected = {
        {2, 0, 0xAA}, {2, 1, 0x55}, {5, 0, 0xAA}, {1, 0, 0x0F}, {3, 0, 0x33},
    };

    Vsystem_top* top = new Vsystem_top;
    int pc = 0;
    uint16_t rom_reg = rom[0];
    std::vector<Write> writes;

    auto rom_at = [&](int a) { return a < rom_size ? rom[a] : (uint16_t)0x0000; };

    // reset
    top->clk = 0; top->rst = 1; top->rom_data = rom[0];
    top->eval();
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();
    top->rst = 0;

    std::printf("--- FULL SYSTEM TEST (%s ROM) ---\n", sync_rom ? "synchrónna" : "kombinačná");

    for (int cycle = 0; cycle < 40 && pc < rom_size; cycle++) {
        top->rom_data = sync_rom ? rom_reg : rom_at(pc);
        top->eval();

        // hodnoty pred hranou = to, čo register file / PC uvidia na tejto hrane
        if (top->write_reg_en)
            writes.push_back({top->reg_addr, top->high_b, top->reg_data});
        bool inc = top->pc_inc;
        std::printf("cyklus %2d | PC=%d ROM=0x%04X | stav=%2d | pc_inc=%d we=%d addr=%d high=%d data=0x%02X\n",
                    cycle, pc, top->rom_data, top->state_out, inc,
                    top->write_reg_en, top->reg_addr, top->high_b, top->reg_data);

        top->clk = 1; top->eval();
        rom_reg = rom_at(pc);        // BRAM zachytí adresu pred hranou
        if (inc) pc++;
        top->clk = 0; top->eval();
    }

    // dobeh posledných zápisov
    for (int i = 0; i < 4; i++) {
        top->rom_data = sync_rom ? rom_reg : rom_at(pc);
        top->eval();
        if (top->write_reg_en) writes.push_back({top->reg_addr, top->high_b, top->reg_data});
        top->clk = 1; top->eval(); top->clk = 0; top->eval();
    }

    int fails = 0;
    for (size_t i = 0; i < expected.size(); i++) {
        const Write& e = expected[i];
        if (i >= writes.size()) {
            std::printf("CHYBA: chýba zápis #%zu (r%d high=%d 0x%02X)\n", i, e.addr, e.high, e.data);
            fails++;
            continue;
        }
        const Write& w = writes[i];
        bool ok = w.addr == e.addr && w.high == e.high && w.data == e.data;
        std::printf("%s zápis #%zu: r%d high=%d 0x%02X (očakávané r%d high=%d 0x%02X)\n",
                    ok ? "OK   " : "CHYBA", i, w.addr, w.high, w.data, e.addr, e.high, e.data);
        if (!ok) fails++;
    }

    std::printf("\n--- %s ---\n", fails ? "TEST ZLYHAL" : "VŠETKY TESTY PREŠLI");
    delete top;
    return fails ? 1 : 0;
}

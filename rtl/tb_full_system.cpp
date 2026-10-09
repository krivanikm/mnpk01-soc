// Testbench celého systému.
//
// Každý test je textový súbor v tests/ (formát popísaný v tests/README).
// Testbench program nahrá do ROM, spustí ho v oboch režimoch pamäte
// a na konci porovná obsah VŠETKÝCH registrov s očakávanými hodnotami.
// Register, ktorý v teste nie je uvedený, musí byť 0.
//
// Použitie:
//   ./Vsystem_top tests/*.txt          spustí testy
//   ./Vsystem_top -v tests/basic.txt   vypíše aj priebeh po taktoch
//
// Režimy ROM (testujú sa vždy oba):
//   kombinačná   rom_data = rom[pc] okamžite
//   sync         ako FPGA BRAM: adresa sa zachytí na hrane, dáta prídu o takt neskôr
//
// PC zatiaľ emuluje testbench (rovnako ako programcounter.v: PC++ na hrane, keď pc_inc = 1).
// Keď bude PC v system_top, stačí čítať PC z neho namiesto premennej pc.

#include <verilated.h>
#include "Vsystem_top.h"
#include "Vsystem_top___024root.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static const int MAX_CYCLES = 10000;   // ochrana proti zaseknutiu / nekonečnej slučke
static const int S_FETCH = 0;          // číslo stavu S_FETCH v controlunit.sv (prvý v enum)

struct Test {
    std::string name;
    std::vector<uint16_t> rom;
    uint16_t expect_reg[16] = {0};
    int expect_pc = -1;                 // -1 = koniec programu (za poslednou inštrukciou)
};

// --- načítanie testu zo súboru ---------------------------------------------

static bool parse_number(const std::string& s, long& out) {
    char* end = nullptr;
    out = std::strtol(s.c_str(), &end, 0);   // podporuje 0x.., aj desiatkové
    return end && *end == '\0' && !s.empty();
}

static bool load_test(const char* path, Test& t) {
    std::ifstream f(path);
    if (!f) {
        std::printf("CHYBA: neviem otvoriť %s\n", path);
        return false;
    }
    t.name = path;

    std::string line;
    int lineno = 0;
    while (std::getline(f, line)) {
        lineno++;
        size_t c = line.find_first_of(";#");          // komentáre
        if (c != std::string::npos) line.erase(c);

        std::istringstream in(line);
        std::string word;
        if (!(in >> word)) continue;                  // prázdny riadok

        if (word == "expect") {
            // expect r2=0x55AA r5=0xAA pc=6
            std::string item;
            while (in >> item) {
                size_t eq = item.find('=');
                long val;
                if (eq == std::string::npos || !parse_number(item.substr(eq + 1), val)) {
                    std::printf("%s:%d: zlý zápis '%s'\n", path, lineno, item.c_str());
                    return false;
                }
                std::string key = item.substr(0, eq);
                long r;
                if (key == "pc") {
                    t.expect_pc = (int)val;
                } else if ((key[0] == 'r' || key[0] == 'R') && parse_number(key.substr(1), r) && r >= 0 && r < 16) {
                    t.expect_reg[r] = (uint16_t)val;
                } else {
                    std::printf("%s:%d: neznámy register '%s'\n", path, lineno, key.c_str());
                    return false;
                }
            }
        } else {
            // inštrukcia: 4 hex číslice, napr. 12AA
            long val;
            if (!parse_number("0x" + word, val) || val < 0 || val > 0xFFFF) {
                std::printf("%s:%d: zlá inštrukcia '%s'\n", path, lineno, word.c_str());
                return false;
            }
            t.rom.push_back((uint16_t)val);
        }
    }
    if (t.expect_pc < 0) t.expect_pc = (int)t.rom.size();
    return true;
}

// --- simulácia -------------------------------------------------------------

static void tick(Vsystem_top* top) {
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();
}

// Vráti true, ak test prešiel.
static bool run(const Test& t, bool sync_rom, bool verbose) {
    Vsystem_top* top = new Vsystem_top;
    auto rom_at = [&](int a) -> uint16_t {
        return (a >= 0 && a < (int)t.rom.size()) ? t.rom[a] : 0x0000;   // mimo programu = NOP
    };

    int pc = 0;
    uint16_t rom_reg = rom_at(0);

    // reset
    top->clk = 0; top->rst = 1; top->rom_data = rom_at(0);
    top->eval();
    tick(top);
    top->rst = 0;

    if (verbose)
        std::printf("  --- %s ROM ---\n", sync_rom ? "synchrónna" : "kombinačná");

    // Beží, kým CU nechce načítať inštrukciu za koncom programu.
    // Na hrane tohto taktu sa ešte zapíše výsledok poslednej inštrukcie
    // (write_reg_en je registrovaný), preto sa končí až po nej.
    int cycle = 0;
    for (; cycle < MAX_CYCLES; cycle++) {
        top->rom_data = sync_rom ? rom_reg : rom_at(pc);
        top->eval();

        bool done = top->state_out == S_FETCH && pc >= (int)t.rom.size();

        if (verbose)
            std::printf("  cyklus %3d | PC=%3d ROM=0x%04X | stav=%2d | pc_inc=%d we=%d%d addr=%2d data=0x%04X\n",
                        cycle, pc, top->rom_data, top->state_out, top->pc_inc,
                        (top->write_reg_en >> 1) & 1, top->write_reg_en & 1, top->reg_addr, top->reg_data);

        bool inc = top->pc_inc;
        top->clk = 1; top->eval();
        rom_reg = rom_at(pc);          // BRAM zachytí adresu, ktorá bola pred hranou
        if (inc) pc++;
        top->clk = 0; top->eval();
        if (done) break;
    }

    bool ok = true;
    const char* mode = sync_rom ? "sync" : "komb";

    if (cycle >= MAX_CYCLES) {
        std::printf("  [%s] CHYBA: program neskončil ani po %d taktoch (PC=%d)\n", mode, MAX_CYCLES, pc);
        ok = false;
    }
    if (pc != t.expect_pc) {
        std::printf("  [%s] CHYBA: PC=%d, očakávané %d\n", mode, pc, t.expect_pc);
        ok = false;
    }

    // skutočný obsah register file (Verilator --public-flat-rw)
    auto& regfile = top->rootp->system_top__DOT__rf_inst__DOT__regfile;
    for (int r = 0; r < 16; r++) {
        uint16_t got = regfile[r];
        if (got != t.expect_reg[r]) {
            std::printf("  [%s] CHYBA: r%-2d = 0x%04X, očakávané 0x%04X\n", mode, r, got, t.expect_reg[r]);
            ok = false;
        }
    }

    if (ok)
        std::printf("  [%s] OK  (%d taktov)\n", mode, cycle);

    delete top;
    return ok;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    bool verbose = false;
    std::vector<const char*> files;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-v") == 0) verbose = true;
        else if (argv[i][0] != '+') files.push_back(argv[i]);   // +args patria Verilatoru
    }
    if (files.empty()) {
        std::printf("Použitie: %s [-v] tests/*.txt\n", argv[0]);
        return 2;
    }

    int passed = 0, failed = 0;
    for (const char* f : files) {
        Test t;
        std::printf("%s\n", f);
        if (!load_test(f, t)) { failed++; continue; }

        bool ok = run(t, false, verbose);
        ok = run(t, true, verbose) && ok;
        if (ok) passed++; else failed++;
    }

    std::printf("\n--- %d/%d testov prešlo%s ---\n", passed, passed + failed,
                failed ? "  ->  TEST ZLYHAL" : "");
    return failed ? 1 : 0;
}

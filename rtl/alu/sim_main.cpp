// Test 16-bitovej ALU.
//
// Všetky kombinácie (65536 x 65536 x 14 operácií) sa nedajú prejsť, preto pre každú operáciu:
//   1. všetky hodnoty a (65536) x hraničné hodnoty b
//   2. hraničné hodnoty a x všetky hodnoty b
//   3. 1 000 000 náhodných dvojíc (a, b)
// Výsledok aj flagy sa porovnajú s hodnotami vypočítanými v C++.

#include <iostream>
#include <cstdint>
#include <random>
#include "Valu.h"
#include "verilated.h"

static const int EDGE[] = {0x0000, 0x0001, 0x0002, 0x007F, 0x0080, 0x00FF, 0x0100, 0x0101,
                           0x7FFF, 0x8000, 0x8001, 0xFF00, 0xFFFE, 0xFFFF};

static Valu* top;
static long total_tests = 0;
static long errors = 0;

static void check(int op, int a, int b) {
    top->a = a;
    top->b = b;
    top->op = op;
    top->eval();

    int exp_out = 0;
    int exp_c = 0;
    int exp_z = 0;
    int exp_n = 0;

    switch (op) {
        case 0:  exp_out = (a + b) & 0xFFFF; exp_c = ((a + b) > 0xFFFF); break;
        case 1:  exp_out = (a - b) & 0xFFFF; exp_c = (a < b); break;
        case 2:  exp_out = (a + 1) & 0xFFFF; exp_c = ((a + 1) > 0xFFFF); break;
        case 3:  exp_out = (a - 1) & 0xFFFF; exp_c = (a < 1); break;
        case 4:  exp_out = (a & b); break;
        case 5:  exp_out = (a | b); break;
        case 6:  exp_out = (a ^ b); break;
        case 7:  exp_out = (~a) & 0xFFFF; break;
        case 8:  exp_out = (a << 1) & 0xFFFF; exp_c = (a >> 15) & 1; break;
        case 9:  exp_out = (a >> 1) & 0xFFFF; exp_c = a & 1; break;
        case 10: exp_out = ((a << 1) | (a >> 15)) & 0xFFFF; exp_c = (a >> 15) & 1; break;
        case 11: // CMP
            exp_out = a;
            exp_z = (a == b);
            exp_c = (a > b);
            exp_n = (a < b);
            break;
        case 12: exp_out = a; break; // PASS_A
        case 13: exp_out = 0; break; // CLR
        default: exp_out = 0; break;
    }

    if (op != 11) {
        exp_z = (exp_out == 0);
        exp_n = (exp_out >> 15) & 1;
    }

    // Kontrola všetkého: OUT, Z, C, N
    bool bug = (top->out != exp_out) ||
               (top->z_flag != exp_z) ||
               (top->c_flag != exp_c) ||
               (top->n_flag != exp_n);

    if (bug) {
        errors++;
        if (errors <= 20) {
            std::cout << " CHYBA: OP:" << op << " A:0x" << std::hex << a << " B:0x" << b
                      << " | RTL[OUT:0x" << (int)top->out << " Z:" << (int)top->z_flag << " C:" << (int)top->c_flag << " N:" << (int)top->n_flag << "]"
                      << " | EXP[OUT:0x" << exp_out << " Z:" << exp_z << " C:" << exp_c << " N:" << exp_n << "]" << std::dec << std::endl;
        }
    }
    total_tests++;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    top = new Valu;

    std::mt19937 rng(12345);   // pevné semienko -> test je vždy rovnaký

    for (int op = 0; op < 14; op++) {
        for (int x = 0; x < 0x10000; x++) {
            for (int e : EDGE) {
                check(op, x, e);
                check(op, e, x);
            }
        }
        for (int i = 0; i < 1000000; i++)
            check(op, rng() & 0xFFFF, rng() & 0xFFFF);
    }

    std::cout << "\n--- REPORT ---" << std::endl;
    std::cout << "Testov celkovo: " << total_tests << std::endl;
    if (errors == 0) {
        std::cout << " STATUS: 100% SUCCESS!" << std::endl;
    } else {
        std::cout << " STATUS: Nájdených " << errors << " chýb. Skontroluj logiku vyššie." << std::endl;
    }

    delete top;
    return errors ? 1 : 0;
}

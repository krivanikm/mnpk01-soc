#include "Vuart_rx.h"
#include "verilated.h"
#include <cstdio>

static const int N = 16;
static Vuart_rx *dut;
static int fails = 0;
static int valid_count = 0;

static void tick() {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
    if (dut->valid) valid_count++;
}

static void hold(int level, int cycles) {
    dut->rx = level;
    for (int i = 0; i < cycles; i++) tick();
}

static void check(bool ok, const char *msg, int byte) {
    if (!ok) { printf("FAIL 0x%02X: %s\n", byte, msg); fails++; }
}

static void send_frame(int byte, int stop_level) {
    hold(0, N);
    for (int b = 0; b < 8; b++) hold((byte >> b) & 1, N);
    hold(stop_level, N);
}

static void send_and_check(int byte) {
    valid_count = 0;
    send_frame(byte, 1);
    hold(1, 4);
    check(valid_count == 1, "valid did not pulse exactly once", byte);
    check(dut->data == byte, "wrong data", byte);
    if (valid_count == 1 && dut->data == byte) printf("ok   0x%02X\n", byte);
    hold(1, 2 * N);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vuart_rx;
    dut->rx = 1;
    dut->rst = 1;
    tick(); tick();
    dut->rst = 0;
    hold(1, 4);
    check(dut->valid == 0, "valid high after reset", 0);

    int bytes[] = {0x41, 0x55, 0xAA, 0x00, 0xFF, 0x80, 0x01};
    for (int b : bytes) send_and_check(b);

    valid_count = 0;
    hold(0, N / 4);
    hold(1, 3 * N);
    check(valid_count == 0, "glitch accepted as start bit", 0);
    printf("ok   glitch rejected\n");

    send_and_check(0x3C);

    valid_count = 0;
    send_frame(0x5A, 1);
    send_frame(0xA5, 1);
    hold(1, 4);
    check(valid_count == 2, "back-to-back frames: expected 2 valid pulses", 0);
    check(dut->data == 0xA5, "back-to-back: last byte wrong", 0xA5);
    if (valid_count == 2 && dut->data == 0xA5) printf("ok   back-to-back\n");

    delete dut;
    if (fails) { printf("%d FAILED\n", fails); return 1; }
    printf("uart_rx: all passed\n");
    return 0;
}

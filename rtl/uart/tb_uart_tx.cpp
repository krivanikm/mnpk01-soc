#include "Vuart_tx.h"
#include "verilated.h"
#include <cstdio>
#include <vector>

static const int N = 16;
static Vuart_tx *dut;
static int fails = 0;

static void tick() {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

static void check(bool ok, const char *msg, int byte) {
    if (!ok) { printf("FAIL 0x%02X: %s\n", byte, msg); fails++; }
}

static void send_and_check(int byte) {
    dut->data = byte;
    dut->start = 1;
    tick();
    dut->start = 0;
    dut->data = 0;

    std::vector<int> tx, busy;
    for (int i = 0; i < 12 * N; i++) {
        tx.push_back(dut->tx);
        busy.push_back(dut->busy);
        tick();
    }

    int t0 = -1;
    for (int i = 0; i < (int)tx.size(); i++)
        if (tx[i] == 0) { t0 = i; break; }
    check(t0 >= 0, "start bit never appeared", byte);
    if (t0 < 0) return;
    check(t0 <= 1, "start bit late", byte);

    int expect[10];
    expect[0] = 0;
    for (int b = 0; b < 8; b++) expect[1 + b] = (byte >> b) & 1;
    expect[9] = 1;

    for (int k = 0; k < 10; k++)
        for (int c = 0; c < N; c++) {
            int i = t0 + k * N + c;
            if (tx[i] != expect[k]) {
                printf("FAIL 0x%02X: bit %d cycle %d tx=%d expected %d\n", byte, k, c, tx[i], expect[k]);
                fails++;
                return;
            }
        }

    int end = t0 + 10 * N;
    for (int i = t0 - 1; i < end - 1; i++)
        check(busy[i] == 1, "busy dropped mid-frame", byte);
    check(busy[end - 1] == 0, "busy still high after frame", byte);
    for (int i = end; i < (int)tx.size(); i++)
        check(tx[i] == 1, "line not idle high after frame", byte);
    printf("ok   0x%02X\n", byte);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vuart_tx;
    dut->rst = 1; dut->start = 0; dut->data = 0;
    tick(); tick();
    dut->rst = 0;
    tick();
    check(dut->tx == 1, "tx not high after reset", 0);
    check(dut->busy == 0, "busy high after reset", 0);

    int bytes[] = {0x41, 0x55, 0x00, 0xFF, 0x80, 0x01};
    for (int b : bytes) send_and_check(b);

    delete dut;
    if (fails) { printf("%d FAILED\n", fails); return 1; }
    printf("uart_tx: all passed\n");
    return 0;
}

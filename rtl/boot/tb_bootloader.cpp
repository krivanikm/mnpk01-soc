#include "Vbootloader.h"
#include "verilated.h"
#include <cstdio>
#include <cstring>

static const int N = 16;
static Vbootloader *dut;
static int fails = 0;
static int wr_addr[512], wr_data[512], wr_n = 0;

static void tick() {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
    if (dut->we && wr_n < 512) { wr_addr[wr_n] = dut->waddr; wr_data[wr_n] = dut->wdata; wr_n++; }
}

static void hold(int level, int cycles) {
    dut->rx = level;
    for (int i = 0; i < cycles; i++) tick();
}

static void send_byte(int byte) {
    hold(0, N);
    for (int b = 0; b < 8; b++) hold((byte >> b) & 1, N);
    hold(1, N);
    hold(1, 4);
}

static void send_str(const char *s) {
    for (size_t i = 0; i < strlen(s); i++) send_byte((unsigned char)s[i]);
}

static void reset() {
    dut->rx = 1;
    dut->rst = 1;
    tick(); tick();
    dut->rst = 0;
    hold(1, 4);
}

static void expect_rst(const char *seq, int want) {
    reset();
    send_str(seq);
    if (dut->cpu_rst != want) {
        printf("FAIL \"%s\": cpu_rst=%d, ocakavane %d\n", seq, dut->cpu_rst, want);
        fails++;
    } else {
        printf("ok   \"%s\" -> cpu_rst=%d\n", seq, want);
    }
}

static void send_prog(const int *words, int n, int chk_flip) {
    wr_n = 0;
    send_str("MNPK");
    send_byte(n >> 8);
    send_byte(n & 0xFF);
    int chk = 0;
    for (int i = 0; i < n; i++) {
        send_byte(words[i] >> 8);
        send_byte(words[i] & 0xFF);
        chk ^= (words[i] >> 8) ^ (words[i] & 0xFF);
    }
    send_byte(chk ^ chk_flip);
}

static bool check_done(const char *name, int ok, int err, int rst) {
    if (dut->done_ok != ok || dut->done_err != err || dut->cpu_rst != rst) {
        printf("FAIL %s: done_ok=%d done_err=%d cpu_rst=%d, ocakavane %d %d %d\n",
               name, dut->done_ok, dut->done_err, dut->cpu_rst, ok, err, rst);
        return false;
    }
    return true;
}

static void upload(const char *name, const int *words, int n) {
    reset();
    send_prog(words, n, 0);
    bool ok = wr_n == n;
    for (int i = 0; ok && i < n; i++)
        if (wr_addr[i] != i || wr_data[i] != words[i]) {
            printf("FAIL %s: zapis %d -> mem[%d]=0x%04X, ocakavane mem[%d]=0x%04X\n",
                   name, i, wr_addr[i], wr_data[i], i, words[i]);
            ok = false;
        }
    if (wr_n != n) printf("FAIL %s: %d zapisov, ocakavane %d\n", name, wr_n, n);
    if (ok) ok = check_done(name, 1, 0, 0);
    if (ok) printf("ok   %s: %d slov na adresy 0..%d, done_ok\n", name, n, n - 1);
    else fails++;
}

static void expect(const char *name, bool ok) {
    if (ok) printf("ok   %s\n", name);
    else fails++;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vbootloader;

    expect_rst("", 0);
    expect_rst("MNPK", 1);
    expect_rst("MNP", 0);
    expect_rst("MNXK", 0);
    expect_rst("NPK", 0);
    expect_rst("MMNPK", 1);
    expect_rst("MNMNPK", 1);
    expect_rst("MNPMNPK", 1);
    expect_rst("MNNPK", 0);
    expect_rst("MNPPK", 0);
    expect_rst("xyzMNPK", 1);
    expect_rst("mnpk", 0);

    int p1[] = {0x1111, 0x2222, 0x3333};
    upload("3 slova", p1, 3);
    int p2[] = {0x3A7F};
    upload("1 slovo", p2, 1);
    int p3[256];
    for (int i = 0; i < 256; i++) p3[i] = (i * 0x0101) ^ 0xA55A;
    upload("256 slov", p3, 256);

    reset();
    expect("po resete done_ok=0 done_err=0", check_done("po resete", 0, 0, 0));

    reset();
    send_prog(p1, 3, 0x01);
    expect("zly checksum -> done_err, CPU ostava v resete", check_done("zly checksum", 0, 1, 1));

    send_str("MNPK");
    expect("nove MNPK po chybe nuluje done_err", check_done("MNPK po chybe", 0, 0, 1));

    reset();
    send_prog(p1, 3, 0x80);
    send_prog(p2, 1, 0);
    expect("zly a potom dobry upload -> done_ok", check_done("zly potom dobry", 1, 0, 0));

    reset();
    send_prog(p1, 3, 0);
    send_prog(p2, 1, 0x10);
    expect("dobry a potom zly upload -> done_err", check_done("dobry potom zly", 0, 1, 1));

    delete dut;
    if (fails) { printf("%d FAILED\n", fails); return 1; }
    printf("bootloader: all passed\n");
    return 0;
}

module top (
    input  wire       clk,
    input  wire [7:0] dip,
    output wire [7:0] ext,
    output wire [5:0] led,
    output wire       uart_tx,
    input wire       uart_rx
);

    wire        boot_we;
    wire [7:0]  boot_waddr;
    wire [15:0] boot_wdata;
    wire        cpu_rst;

    reg [15:0] rom [0:255];
    initial $readmemh("prog.hex", rom);

    reg [3:0] rst_cnt = 0;
    wire      rst     = ~&rst_cnt;
    always @(posedge clk) if (rst) rst_cnt <= rst_cnt + 1;
    always @(posedge clk) if (boot_we) rom[boot_waddr] <= boot_wdata;

    wire [15:0] pc;
    wire [15:0] io_out;
    wire        w_halted;

    reg [7:0] dip_s1 = 8'hFF;
    reg [7:0] dip_s2 = 8'hFF;

    assign led = {5'b11111, ~w_halted};

    always @(posedge clk) begin
        dip_s1 <= dip;
        dip_s2 <= dip_s1;
    end

    system_top cpu (
        .clk(clk),
        .rst(rst | cpu_rst),
        .rom_data(rom[pc[7:0]]),
        .io_in({8'h00, ~dip_s2}),
        .state_out(),
        .pc_inc(),
        .write_reg_en(),
        .reg_addr(),
        .reg_data(),
        .reg_q_out(),
        .pc(pc),
        .io_out(io_out),
        .uart_tx_pin(uart_tx),
        .uart_rx_pin(uart_rx),
        .halted(w_halted)
    );

    bootloader boot (
      .clk(clk),
      .rst(rst),
      .rx(uart_rx),
      .cpu_rst(cpu_rst),
      .we(boot_we),
      .waddr(boot_waddr),
      .wdata(boot_wdata),
      .done_ok(),
      .done_err()
    );

    assign ext = io_out[7:0];

endmodule

module top (
    input  wire       clk,
    input  wire [7:0] dip,
    output wire [7:0] ext,
    output wire [5:0] led,
    output wire       uart_tx
);

    reg [3:0] rst_cnt = 0;
    wire      rst     = ~&rst_cnt;
    always @(posedge clk) if (rst) rst_cnt <= rst_cnt + 1;

    reg [15:0] rom [0:255];
    initial $readmemh("prog.hex", rom);

    wire [15:0] pc;
    wire [15:0] io_out;

    reg [7:0] dip_s1 = 8'hFF;
    reg [7:0] dip_s2 = 8'hFF;
    always @(posedge clk) begin
        dip_s1 <= dip;
        dip_s2 <= dip_s1;
    end

    system_top cpu (
        .clk(clk),
        .rst(rst),
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
        .uart_tx_pin(uart_tx)
    );

    assign ext = io_out[7:0];
    assign led = 6'b111111;

endmodule

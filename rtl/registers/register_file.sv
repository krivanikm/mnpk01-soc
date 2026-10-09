module register_file(
    input wire clk,
    input wire rst_n,
    input wire [3:0] addr,
    input wire [15:0] d_in,
    input wire [1:0] write_en,  // [1] = zapíš horný bajt, [0] = zapíš dolný bajt, 2'b11 = celé slovo
    output [15:0] q_out,
    output [3:0] r1_out
);

reg [15:0] regfile [15:0];
integer i;

assign q_out = regfile[addr];
assign r1_out = regfile[4'b0001][3:0];


always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        for (i = 0; i < 16; i = i + 1)
            regfile[i] <= 16'h0000;
    end else begin
        if (write_en[1])
            regfile[addr][15:8] <= d_in[15:8];
        if (write_en[0])
            regfile[addr][7:0] <= d_in[7:0];
    end
end

endmodule

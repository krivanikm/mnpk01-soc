module register_file(
    input wire clk,
    input wire rst_n,
    input wire [3:0] addr,
    input wire high_b,
    input wire [7:0] d_in,
    input wire write_en,
    output [7:0] q_out
);

reg [15:0] regfile [15:0];
integer i;

assign q_out = high_b ? regfile[addr][15:8] : regfile[addr][7:0];

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        for (i = 0; i < 16; i = i + 1)
            regfile[i] <= 16'h0000;
    end else if (write_en) begin
        if (high_b)
            regfile[addr][15:8] <= d_in;
        else
            regfile[addr][7:0] <= d_in;
    end
end

endmodule

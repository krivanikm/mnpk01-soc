module data_ram(
    input wire          clk,
    input wire [11:0]   addr,
    input wire          we,
    input wire [15:0]   din,
    output reg [15:0]   dout
);
    reg [15:0] mem [0:4095];

    always @(posedge clk)begin
        if(we)
            mem[addr] <= din;
        dout <= mem[addr];
    end
endmodule
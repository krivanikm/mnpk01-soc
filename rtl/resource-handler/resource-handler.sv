module resourcehandler(
    input [7:0] d_in,
    input [7:0] reg_data,
    input reg_passthrough,
    input clk,
    input rst,
    input [3:0] reg_addr_write,
    input [3:0] reg_addr_read,
    output reg [7:0] reg_in
);

always @(posedge clk or posedge rst) begin
    if (rst) begin
        reg_in <= 8'h00;
    end else if (reg_passthrough) begin
        reg_in <= d_in;
    end
end

endmodule
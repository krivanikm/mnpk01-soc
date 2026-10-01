module programcounter (
    input wire clk,
    input wire rst,
    input wire pc_inc,
    input wire pc_load,
    input wire [15:0] d_in,
    output reg [15:0] pc
);
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            pc <= 16'h0;
            
        end else begin

        if(pc_inc) begin
            pc <= pc + 16'h1;
        end

        else if(pc_load)begin
            pc <= d_in;

        end
    end

        
    end
endmodule

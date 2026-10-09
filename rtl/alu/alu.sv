module alu (
    input [15:0] a,
    input [15:0] b,
    input [3:0] op,
    output reg [15:0] out,
    output reg z_flag,
    output reg c_flag,
    output reg n_flag
);
    reg [16:0] tmp_result;

    always @(*) begin
        tmp_result = 17'b0;
        out = 16'b0;
        c_flag = 1'b0;

        case (op)
        4'b0000: begin  //ADD
             tmp_result = a+b;
             out = tmp_result[15:0];
             c_flag = tmp_result[16];
        end

        4'b0001: begin  //SUB
            out = a-b;
            c_flag = (a<b);
        end

        4'b0010: begin  //INC A
            tmp_result = a+1;
            out = tmp_result[15:0];
            c_flag = tmp_result[16];
        end

        4'b0011: begin //DEC A
            out = a-1;
            c_flag = (a<1);
        end

        4'b0100: begin //AND
            out = a&b;
        end

        4'b0101: begin //OR
            out = a|b;
        end

        4'b0110: begin //XOR
            out = a^b;
        end

        4'b0111: begin //NOT
            out = ~a;
        end

        4'b1000: begin //SHL
            c_flag = a[15];
            out = a<<1;
        end

        4'b1001: begin //SHR
            c_flag = a[0];
            out = a>>1;
        end

        4'b1010: begin //ROL
            out = {a[14:0], a[15]};
            c_flag = a[15];
        end

        4'b1011: begin //CMP
            out = a;
            c_flag = (a>b);
        end

        4'b1100: begin //PASS_A
            out = a;
        end

        4'b1101: begin //CLR
            out = 16'b0;
        end

        default: begin
            out = 16'b0;
        end
        endcase

        z_flag = (out == 16'b0);
        n_flag = out[15];
        
        if (op == 4'b1011) begin
             z_flag = (a == b);
             n_flag = (a < b);
        end
    end

endmodule

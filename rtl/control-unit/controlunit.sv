`define OP_NOP   8'h00
`define OP_MVI   8'h01
`define OP_MOV   8'h02
`define OP_MVIB  8'h03
`define OP_LOAD  8'h04
`define OP_STORE 8'h05

module controlunit (
    input clk,
    input rst,
    input [15:0] rom_data,
    output reg pc_inc,
    output reg write_reg_en,
    output reg [3:0] reg_addr,
    output reg write_temp_from_reg,
    output reg high_b,
    output reg 
);

    reg [3:0] bytes_to_fetch;

    // 3 stavy pre flexibilné načítavanie viacbajtových inštrukcií
    typedef enum reg [1:0] {
        S_FETCH   = 2'b00,
        S_DECODE  = 2'b01,
        S_COLLECT = 2'b10
    } state_t;
    
    state_t state;
    reg [7:0] ir;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state               <= S_FETCH;
            ir                  <= 8'h00;
            bytes_to_fetch      <= 0;
            pc_inc              <= 0;
            write_reg_en        <= 0;
            reg_addr            <= 4'h0;
            write_temp_from_reg <= 0;
            high_b              <= 0;
        end else begin
            // Predvolené (default) hodnoty, aby nevznikali latche
            pc_inc       <= 0;
            write_reg_en <= 0;

            case (state)
                S_FETCH: begin  
                    ir             <= rom_data[15:12];  
                    state          <= S_DECODE;
                end
                
                S_DECODE: begin  
                    if (ir == `OP_MVI) begin
                        state          <= S_COLLECT;
                    end else begin
                        state <= S_FETCH;
                    end
                end
                
                S_COLLECT: begin
                    if (ir == `OP_MVI) begin
                        pc_inc         <= 1;
                        reg_addr       <= rom_data[3:0];
                        state          <= S_COLLECT;
                    end
                    if(ir = )
                end
                
                default: state <= S_FETCH;
            endcase
        end
    end
     
endmodule 
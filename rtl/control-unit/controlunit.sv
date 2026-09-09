`define OP_NOP   8'h00
`define OP_MVI   8'h01
`define OP_MOV   8'h02
`define OP_MVIB  8'h03
`define OP_LOAD  8'h04
`define OP_STORE 8'h05

module controlunit (
    input  wire        clk,
    input  wire        rst,
    input  wire [15:0] rom_data,
    output reg         pc_inc,
    output reg         write_reg_en,
    output reg  [3:0]  reg_addr,
    output reg         write_temp_from_reg,
    output reg  [7:0]  reg_data,        // dáta pre zápis do registra
    output reg         high_b,
    output reg [1:0]   state_out
);

    typedef enum reg [1:0] {
        S_FETCH   = 2'b00,
        S_DECODE  = 2'b01,
        S_COLLECT = 2'b10
    } state_t;

    state_t   state;
    reg [7:0] ir;
    reg [3:0] bytes_to_fetch;  
    reg [7:0] temp_reg;

    // Prepojenie vnútorného stavu na výstup pre testbench
    assign state_out = state;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state               <= S_FETCH;
            ir                  <= 8'h00;
            bytes_to_fetch      <= 4'h0;
            pc_inc              <= 1'b0;
            write_reg_en        <= 1'b0;
            reg_addr            <= 4'h0;
            reg_data            <= 8'h00;
            write_temp_from_reg <= 1'b0;
            high_b              <= 1'b0;
        end else begin
            // predvolené hodnoty - pulzné signály trvajú 1 takt
            pc_inc              <= 1'b0;
            write_reg_en        <= 1'b0;
            write_temp_from_reg <= 1'b0;
            high_b              <= 1'b0;

            case (state)
                S_FETCH: begin
                    ir     <= rom_data[15:8];   
                    pc_inc <= 1'b1;
                    state  <= S_DECODE;
                end

                S_DECODE: begin
                    case (ir)
                        `OP_MVI, `OP_MVIB: state <= S_COLLECT;

                        `OP_MOV: begin
                            write_reg_en <= 1'b1;
                            reg_addr     <= rom_data[7:4];
                            state        <= S_FETCH;
                        end

                        `OP_LOAD, `OP_STORE: begin
                            state <= S_FETCH;
                        end

                        default: state <= S_FETCH; 
                    endcase
                end

                S_COLLECT: begin
                    write_reg_en <= 1'b1;
                    reg_addr     <= rom_data[15:12];
                    reg_data     <= rom_data[11:4];
                    pc_inc       <= 1'b1;
                    state        <= S_FETCH;       
                    case(ir)
                        `OP_MVIB: begin
                            high_b <= 1'b1;
                        end
                        default: high_b <= 1'b0;
                    endcase
                end

                default: state <= S_FETCH;
            endcase
        end
    end

endmodule
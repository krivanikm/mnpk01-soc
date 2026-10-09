    `define OP_NOP   4'h0
    `define OP_MVI   4'h1
    `define OP_MOV   4'h2
    `define OP_MVIB  4'h3
    `define OP_LOAD  4'h4
    `define OP_STORE 4'h5
    `define OP_JMP   4'h6
    `define OP_ALU   4'h7
    

    module controlunit (
        input  wire        clk,
        input  wire        rst,
        input  wire [15:0] rom_data,
        input wire [15:0] reg_read_data,
        input wire z_flag,
        input wire c_flag,
        input wire n_flag,
        input wire [15:0] alu_result,
        input wire [3:0] alu_op,
        output reg [15:0] alu_a,
        output reg         pc_inc,
        output reg         pc_load,
        output reg [15:0]  pc_addr,
        output reg  [1:0]  write_reg_en,    // [1] = horný bajt, [0] = dolný bajt, 2'b11 = celé slovo
        output reg  [3:0]  reg_addr,
        output reg  [15:0] reg_data,        // dáta pre zápis do registra
        output wire [3:0]  state_out
    );

        typedef enum reg [3:0] {
        S_FETCH,
        S_DECODE,
        S_NOP,

        S_MVI_WRITE,

        S_MOV_LATCH,
        S_MOV_WRITE,

        S_PC_ADDR_LOAD,

        S_ALU_A,
        S_ALU_B

        }state_t;

        state_t state;

        reg [15:0] ir; 
        reg [15:0] temp_reg;
        reg [2:0] flags;

        // Prepojenie vnútorného stavu na výstup pre testbench
        assign state_out = state;

        always @(posedge clk or posedge rst) begin
            if (rst) begin
                state               <= S_FETCH;
                ir                  <= 16'h0000;
                pc_inc              <= 1'b0;
                write_reg_en        <= 2'b00;
                reg_addr            <= 4'h0;
                reg_data            <= 16'h0000;
                temp_reg            <= 16'h0000;
                flags               <= 3'b000;
                alu_a               <= 16'h0000;
            end else begin
                // predvolené hodnoty - pulzné signály trvajú 1 takt
                pc_inc              <= 1'b0;
                write_reg_en        <= 2'b00;

                case (state)
                    S_FETCH: begin
                        ir     <= rom_data;
                        state  <= S_DECODE;
                        pc_inc <=1;
                    end

                    S_DECODE: begin
                        case (ir[15:12])
                            `OP_MVI, `OP_MVIB:begin 
                                state <= S_MVI_WRITE;
                                end
                            `OP_MOV:begin
                                reg_addr<= ir[7:4];
                                state <= S_MOV_LATCH;
                                end
                            `OP_ALU: begin
                                reg_addr <= ir[11:8];
                                state <= S_ALU_A;
                             end
                            `OP_LOAD: state <= S_NOP; 
                            `OP_STORE: state <= S_NOP;
                            `OP_JMP:begin 
                                state <= S_NOP;
                            end
                            default:begin // NOP + neznámy opcode: prázdny takt, kým sa PC posunie
                                state <= S_NOP;
                            end
                        endcase
                    end

                    S_NOP: begin
                        state <= S_FETCH;
                    end

                    S_MVI_WRITE: begin
                        reg_addr     <= ir[11:8];            // Adresa registra z [11:8]
                        reg_data     <= {ir[7:0], ir[7:0]};  // konštanta na oboch bajtoch, write_reg_en vyberie, ktorý sa zapíše
                        if (ir[15:12] == `OP_MVIB)
                            write_reg_en <= 2'b10;           // MVIB: horný bajt
                        else
                            write_reg_en <= 2'b01;           // MVI: dolný bajt
                        state        <= S_FETCH;
                    end
                    S_MOV_LATCH: begin
                        temp_reg <= reg_read_data;
                        reg_addr <= ir[11:8];
                        state <= S_MOV_WRITE;
                    end
                    S_MOV_WRITE: begin
                        write_reg_en <= 2'b11;
                        reg_data <= temp_reg;
                        state <= S_FETCH;
                    end
                    S_ALU_A: begin 
                        alu_a <= reg_read_data;
                        reg_addr <= ir[7:4];
                        state <= S_ALU_B;
                    end
                    S_ALU_B: begin
                        flags[2]<= z_flag;
                        flags[1]<= c_flag;
                        flags[0]<= n_flag;
                        if (alu_op != 4'b1011) begin
                            write_reg_en <= 2'b11;
                            reg_addr <= ir[3:0];
                            reg_data <= alu_result;
                        end
                        state <= S_FETCH;

                     end
                    default: state <= S_FETCH;
                endcase
            end
        end
    endmodule

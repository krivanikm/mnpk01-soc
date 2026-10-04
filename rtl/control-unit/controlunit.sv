    `define OP_NOP   4'h0
    `define OP_MVI   4'h1
    `define OP_MOV   4'h2
    `define OP_MVIB  4'h3
    `define OP_LOAD  4'h4
    `define OP_STORE 4'h5
    `define OP_JMP   4'h6

    module controlunit (
        input  wire        clk,
        input  wire        rst,
        input  wire [15:0] rom_data,
        input wire [7:0] reg_read_data,
        input wire z_flag,
        input wire c_flag,
        input wire n_flag,    
        output reg         pc_inc,
        output reg         pc_load,
        output reg [15:0]  pc_addr,
        output reg         write_reg_en,
        output reg  [3:0]  reg_addr,
        output reg  [7:0]  reg_data,        // dáta pre zápis do registra
        output reg         high_b,
        output wire [3:0]  state_out
    );

        typedef enum reg [3:0] {
        S_FETCH,
        S_DECODE,
        S_NOP,

        S_MVI_WRITE,

        S_MOV_LATCH,
        S_MOV_WRITE,

        S_PC_ADDR_LOAD


        }state_t;

        state_t state;

        reg [15:0] ir; 
        reg [7:0] temp_reg;

        // Prepojenie vnútorného stavu na výstup pre testbench
        assign state_out = state;

        always @(posedge clk or posedge rst) begin
            if (rst) begin
                state               <= S_FETCH;
                ir                  <= 16'h0000;
                pc_inc              <= 1'b0;
                write_reg_en        <= 1'b0;
                reg_addr            <= 4'h0;
                reg_data            <= 8'h00;
                high_b              <= 1'b0;
                temp_reg <= 8'h00;
            end else begin
                // predvolené hodnoty - pulzné signály trvajú 1 takt
                pc_inc              <= 1'b0;
                write_reg_en        <= 1'b0;
                high_b              <= 1'b0;

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
                        write_reg_en <= 1'b1;
                        reg_addr     <= ir[11:8];   // Adresa registra z [11:8]
                        reg_data     <= ir[7:0];    // Plných 8 bitov dát z dolného bytu [7:0]
                        state        <= S_FETCH;       
                        case(ir[15:12])
                            `OP_MVIB: begin
                                high_b <= 1'b1;
                            end
                            default: high_b <= 1'b0;
                        endcase
                        state        <= S_FETCH;
                    end
                    S_MOV_LATCH: begin
                        temp_reg[7:0] <= reg_read_data;
                        reg_addr <= ir[11:8];
                        state <= S_MOV_WRITE;
                    end
                    S_MOV_WRITE: begin
                        write_reg_en <= 1;
                        reg_data <= temp_reg[7:0];
                        state <= S_FETCH;
                    end
                    default: state <= S_FETCH;
                endcase
            end
        end
    endmodule

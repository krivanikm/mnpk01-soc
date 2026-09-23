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
    output reg         write_temp_from_reg,
    output reg  [7:0]  reg_data,        // dáta pre zápis do registra
    output reg         high_b,
    output reg [1:0]   state_out
);

    typedef enum reg [3:0] {
    S_FETCH,
    S_DECODE,

    S_MVI_WRITE,

    S_MOV_READ,
    S_MOV_LATCH,
    S_MOV_WRITE,

    S_LOAD_ADDR,
    S_LOAD_WAIT,
    S_LOAD_WRITE,

    S_STORE_ADDR,
    S_STORE_READ,
    S_STORE_WRITE,

    S_PC_ADDR_REG_READ,
    S_PC_ADDR_LOAD,


    }state_t;

    state_t state;

    reg [3:0] ir;        // Zmenené na 4-bit, keďže opcode má 4 bity
    reg [3:0] bytes_to_fetch;  
    reg [15:0] temp_reg;

    // Prepojenie vnútorného stavu na výstup pre testbench
    assign state_out = state;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state               <= S_FETCH;
            ir                  <= 4'h0;
            bytes_to_fetch      <= 4'h0;
            pc_inc              <= 1'b0;
            write_reg_en        <= 1'b0;
            reg_addr            <= 4'h0;
            reg_data            <= 8'h00;
            write_temp_from_reg <= 1'b0;
            high_b              <= 1'b0;
            temp_reg <= 16'h00;
        end else begin
            // predvolené hodnoty - pulzné signály trvajú 1 takt
            pc_inc              <= 1'b0;
            write_reg_en        <= 1'b0;
            write_temp_from_reg <= 1'b0;
            high_b              <= 1'b0;

            case (state)
                S_FETCH: begin
                    ir     <= rom_data[15:12];  // Opcode je v horných 4 bitoch
                    state  <= S_DECODE;
                end

                S_DECODE: begin
                    case (ir)
                        `OP_MVI, `OP_MVIB:begin 
                            state <= S_MVI_WRITE;
                            pc_inc <= 1;
                            end
                        `OP_MOV:begin
                            reg_addr<= rom_data[7:4];
                            state <= S_MOV_LATCH;
                            end
                        `OP_LOAD: state <= S_LOAD_ADDR; 
                        `OP_STORE: state <= S_STORE_ADDR;
                        `OP_JMP:begin 
                            state <= S_PC_ADDR_LOAD;
                            pc_inc <= 1;
                        end
                        default: state <= S_FETCH; 
                    endcase
                end

                S_MVI_WRITE: begin
                    write_reg_en <= 1'b1;
                    reg_addr     <= rom_data[11:8];   // Adresa registra z [11:8]
                    reg_data     <= rom_data[7:0];    // Plných 8 bitov dát z dolného bytu [7:0]
                    state        <= S_FETCH;       
                    case(ir)
                        `OP_MVIB: begin
                            high_b <= 1'b1;
                        end
                        default: high_b <= 1'b0;
                    endcase
                    state        <= S_FETCH;
                end
                S_MOV_LATCH: begin
                    temp_reg[7:0] <= reg_read_data;
                    reg_addr <= rom_data[11:8];
                    state <= S_MOV_WRITE;
                    pc_inc <= 1;
                end
                S_MOV_WRITE: begin
                    write_reg_en <= 1;
                    reg_data <= temp_reg;
                    state <= S_FETCH;
                end
                S_PC_ADDR_REG_READ: begin 
                    reg_addr<= rom_data[11:8];
                    temp_reg[7:0] <= reg_read_data;
                end

                default: state <= S_FETCH;
            endcase
        end
    end
temp_reg
endmodule
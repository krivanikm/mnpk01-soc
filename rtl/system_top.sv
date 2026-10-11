module system_top (
    input  wire        clk,
    input  wire        rst,
    input  wire [15:0] rom_data,
    
    output wire [4:0]  state_out,
    output wire        pc_inc,
    output wire [1:0]  write_reg_en,
    output wire [3:0]  reg_addr,
    output wire [15:0] reg_data,
    output wire [15:0] reg_q_out,
    output wire [15:0] pc
     
);

    wire        w_pc_inc;
    wire [1:0]  w_write_reg_en;
    wire [3:0]  w_reg_addr;
    wire [15:0] w_reg_data;
    wire [15:0] w_reg_q_out;

    wire [3:0]  w_alu_op;
    wire [15:0] w_alu_a;
    wire [15:0] w_alu_out;
    wire        w_z_flag;
    wire        w_c_flag;
    wire        w_n_flag;
    wire        w_pc_load;
    wire [15:0] w_pc_addr;
    wire [15:0] w_mem_addr;
    wire        w_mem_we;
    wire [15:0] w_mem_rdata;
    wire [15:0] w_mem_wdata;    

    assign pc_inc       = w_pc_inc;
    assign write_reg_en = w_write_reg_en;
    assign reg_addr     = w_reg_addr;
    assign reg_data     = w_reg_data;
    assign reg_q_out    = w_reg_q_out;

    // Instancia Control Unit
    controlunit cu_inst (
        .clk(clk),
        .rst(rst),
        .rom_data(rom_data),
        .reg_read_data(w_reg_q_out),
        .z_flag(w_z_flag),
        .c_flag(w_c_flag),
        .n_flag(w_n_flag),
        .alu_a(w_alu_a),
        .alu_result(w_alu_out),
        .alu_op(w_alu_op),
        .pc_inc(w_pc_inc),
        .write_reg_en(w_write_reg_en),
        .reg_addr(w_reg_addr),
        .reg_data(w_reg_data),
        .state_out(state_out),
        .pc_load(w_pc_load),
        .pc_addr(w_pc_addr),
        .mem_addr(w_mem_addr),
        .mem_wdata(w_mem_wdata),
        .mem_we(w_mem_we),
        .mem_rdata(w_mem_rdata)
    );

    // Instancia Register File
    register_file rf_inst (
        .clk(clk),
        .rst_n(~rst),
        .addr(w_reg_addr),
        .d_in(w_reg_data),
        .write_en(w_write_reg_en),
        .q_out(w_reg_q_out),
        .r1_out(w_alu_op)
    );

    // Instancia ALU
    alu alu_inst (
        .a(w_alu_a),
        .b(w_reg_q_out),
        .op(w_alu_op),
        .out(w_alu_out),
        .z_flag(w_z_flag),
        .c_flag(w_c_flag),
        .n_flag(w_n_flag)
    );
    programcounter programcounter_inst(
        .clk(clk),
        .rst(rst),
        .pc_inc(w_pc_inc),
        .pc_load(w_pc_load),
        .d_in(w_pc_addr),
        .pc(pc)
    );
    data_ram ram_inst(
        .clk(clk),
        .addr(w_mem_addr[11:0]),
        .din(w_mem_wdata),
        .we(w_mem_we),
        .dout(w_mem_rdata)
    );


endmodule

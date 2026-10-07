module system_top (
    input  wire        clk,
    input  wire        rst,
    input  wire [15:0] rom_data,
    
    output wire [3:0]  state_out,
    output wire        pc_inc,
    output wire        write_reg_en,
    output wire [3:0]  reg_addr,
    output wire        high_b,
    output wire [7:0]  reg_data,
    output wire [7:0]  reg_q_out
);

    // Vnútorné prepojenia
    wire        w_pc_inc;
    wire        w_write_reg_en;
    wire [3:0]  w_reg_addr;
    wire [7:0]  w_reg_data;
    wire        w_high_b;
    wire [7:0]  w_reg_q_out;

    wire [3:0]  w_alu_op;
    wire [7:0]  w_alu_a;
    wire [7:0]  w_alu_out;
    wire        w_z_flag;
    wire        w_c_flag;
    wire        w_n_flag;

    // Prepojenie na výstupy pre C++
    assign pc_inc       = w_pc_inc;
    assign write_reg_en = w_write_reg_en;
    assign reg_addr     = w_reg_addr;
    assign high_b       = w_high_b;
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
        .high_b(w_high_b),
        .state_out(state_out)
    );

    // Instancia Register File
    register_file rf_inst (
        .clk(clk),
        .rst_n(~rst),
        .addr(w_reg_addr),
        .high_b(w_high_b),
        .d_in(w_reg_data),
        .write_en(w_write_reg_en),
        .q_out(w_reg_q_out), // <-- Tu vyťahujeme prečítanú hodnotu
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

endmodule

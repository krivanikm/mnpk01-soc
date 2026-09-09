module system_top (
    input  wire        clk,
    input  wire        rst,
    input  wire [15:0] rom_data,
    
    // Výstupy pre C++ testbench
    output wire [1:0]  state_out,
    output wire        pc_inc,
    output wire        write_reg_en,
    output wire [3:0]  reg_addr,
    output wire        high_b,
    output wire [7:0]  reg_data,
    output wire [7:0]  reg_q_out   // <-- Nový výstup na čítanie z registra
);

    // Vnútorné prepojenia
    wire        w_pc_inc;
    wire        w_write_reg_en;
    wire [3:0]  w_reg_addr;
    wire        w_write_temp_from_reg;
    wire [7:0]  w_reg_data;
    wire        w_high_b;
    wire [7:0]  w_reg_q_out;

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
        .pc_inc(w_pc_inc),
        .write_reg_en(w_write_reg_en),
        .reg_addr(w_reg_addr),
        .write_temp_from_reg(w_write_temp_from_reg),
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
        .q_out(w_reg_q_out) // <-- Tu vyťahujeme prečítanú hodnotu
    );

endmodule
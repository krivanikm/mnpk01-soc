module bootloader #(
    parameter CLKS_PER_BIT = 234
) (
    input wire clk,
    input wire rst,
    input wire rx,
    output reg cpu_rst,
    output reg we,
    output reg [7:0] waddr,
    output reg [15:0] wdata,
    output reg done_ok,
    output reg done_err
);
  wire [7:0] rx_byte;
  wire       rx_valid;
  reg  [7:0] hi;
  reg  [7:0] chk;
  reg  [7:0] addr;
  uart_rx #(
      .CLKS_PER_BIT(CLKS_PER_BIT)
  ) u_uart_rx (
      .clk(clk),
      .rst(rst),
      .rx(rx),
      .data(rx_byte),
      .valid(rx_valid)
  );

  typedef enum reg [3:0] {
    IDLE,
    GOT_M,
    GOT_N,
    GOT_P,
    LEN_HI,
    LEN_LO,
    DATA_HI,
    DATA_LO,
    CHK
  } state_t;
  state_t state;

  reg [15:0] len;

  always @(posedge clk) begin
    if (rst) begin
      state <= IDLE;
      cpu_rst <= 0;
      we <= 0;
      done_ok <= 0;
      done_err <= 0;
    end else begin
      we <= 0;
      if (rx_valid) begin

        case (state)
          IDLE: begin
            if (rx_byte == "M") begin
              state <= GOT_M;
            end else begin
              state <= IDLE;
            end
          end
          GOT_M: begin
            if (rx_byte == "N") begin
              state <= GOT_N;

            end else if (rx_byte == "M") begin
              state <= GOT_M;
            end else begin
              state <= IDLE;
            end

          end
          GOT_N: begin
            if (rx_byte == "P") begin
              state <= GOT_P;
            end else if (rx_byte == "M") begin
              state <= GOT_M;
            end else begin
              state <= IDLE;
            end
          end
          GOT_P: begin
            if (rx_byte == "K") begin
              cpu_rst <= 1;
              done_ok <= 0;
              done_err <= 0;
              state <= LEN_HI;
            end else if (rx_byte == "M") begin
              state <= GOT_M;
            end else begin
              state <= IDLE;
            end


          end
          LEN_HI: begin
            state <= LEN_LO;
            len[15:8] <= rx_byte;
          end
          LEN_LO: begin
            state <= DATA_HI;
            len[7:0] <= rx_byte;
            addr <= 0;
            chk <= 0;
            if ({len[15:8], rx_byte} == 0 || {len[15:8], rx_byte} > 256) begin
              done_err <= 1;
              state <= IDLE;
            end else begin
              state <= DATA_HI;
            end
          end
          DATA_HI: begin
            hi <= rx_byte;
            chk <= chk ^ rx_byte;
            state <= DATA_LO;
          end
          DATA_LO: begin
            wdata <= {hi, rx_byte};
            waddr <= addr;
            we <= 1;
            addr <= addr + 1;
            chk <= chk ^ rx_byte;
            len <= len - 1;

            if (len == 1) begin
              state <= CHK;
            end else begin
              state <= DATA_HI;
            end


          end
          CHK: begin
            if (rx_byte == chk) begin
              done_ok <= 1;
              cpu_rst <= 0;

            end else begin
              done_err <= 1;
            end
            state <= IDLE;
          end
          default: state <= IDLE;
        endcase
      end
    end

  end
endmodule



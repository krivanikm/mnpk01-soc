module uart_tx #(parameter CLKS_PER_BIT = 234)(
  input wire        clk,
  input wire        rst,
  input wire [7:0] data,
  input wire        start,
  output reg        tx,
  output wire       busy
  );

  typedef enum reg [1:0]{
    IDLE,
    START,
    DATA,
    STOP
    }state_t;
    state_t state;

    reg [15:0] clk_cnt;
    reg [2:0] bit_idx;
    reg [7:0] shift;

    assign busy = (state !=IDLE);
    always @(posedge clk) begin
      if(rst) begin
        state<=IDLE;
        tx <=1;
        clk_cnt <=0;
        bit_idx <=0;

      end else begin
        case (state)
          IDLE: begin
            tx <=1;
            if (start) begin
              shift <=data;
              clk_cnt <=0;
              state <=START;
            end
          end
          START: begin
            tx <=0;
            if(clk_cnt == (CLKS_PER_BIT - 1)) begin
              clk_cnt <=0;
              bit_idx <=0;
              state <=DATA;
            end else begin
              clk_cnt <=clk_cnt + 1;
            end
          end
          DATA: begin
            tx <= shift[bit_idx];
            if (clk_cnt == (CLKS_PER_BIT -1)) begin
              clk_cnt <=0;
              if(bit_idx == 7) begin
                state <=STOP;
              end else begin
                bit_idx <=bit_idx + 1;
              end
            end else begin
              clk_cnt <=clk_cnt + 1;
            end

          end
          STOP: begin
            tx <=1;
            if(clk_cnt == (CLKS_PER_BIT-1)) begin
              clk_cnt <=0;
              state <=IDLE;
            end else begin
              clk_cnt <=clk_cnt + 1;
            end
          end

          



  

        endcase
      end
    end





endmodule


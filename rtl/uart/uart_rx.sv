module uart_rx #(parameter CLKS_PER_BIT = 234)(
  input wire clk, rst,
  input wire rx,
  output reg [7:0] data,
  output reg valid
  );

  reg rx_sync1, rx_sync2;
  reg [15:0] clk_cnt;
  reg [2:0] bit_idx;
  
  typedef enum reg[1:0]{
    IDLE,
    START,
    DATA,
    STOP}state_t;
    state_t state;
  
    always @(posedge clk) begin
      rx_sync1 <=rx;
      rx_sync2 <=rx_sync1;

    end
    
    always @(posedge clk) begin
      if(rst) begin
        state <=IDLE;
        clk_cnt <=0;
        bit_idx <=0;
        valid <=0;
        data <=0;
      end else begin

      case (state)
        IDLE: begin
          valid <=0;
          if(rx_sync2 == 0)begin
            clk_cnt <=0;
            state <=START;
          end
        end
        START: begin
          if(clk_cnt == CLKS_PER_BIT/2 -1)begin
            if(rx_sync2 == 0)begin
              clk_cnt <=0;
              bit_idx <=0;
              state <=DATA;
            end else begin
              state <=IDLE;
            end
          end else begin 
            clk_cnt <=clk_cnt + 1;
          end
        end
        DATA: begin
          if(clk_cnt == CLKS_PER_BIT-1) begin
            clk_cnt <=0;
            bit_idx <=bit_idx + 1;
            data[bit_idx] <=rx_sync2;
            if(bit_idx == 7) begin
              state <=STOP;
              clk_cnt <=0;
              bit_idx <=0;
            end
          end else begin
            clk_cnt <=clk_cnt + 1;
          end


        end
        STOP:begin
          if(clk_cnt == CLKS_PER_BIT - 1) begin
            valid <=1;
            state <=IDLE;
          end else begin
            clk_cnt <=clk_cnt + 1;
          end

          

        end


      endcase
      end
    end


endmodule



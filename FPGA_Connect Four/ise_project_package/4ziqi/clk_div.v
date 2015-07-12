// clock divider
module clk_div (clock_40MHz, clock_1MHz, clock_100KHz, clock_10KHz, clock_1KHz, clock_100Hz, clock_10Hz, clock_1Hz);
  
   input clock_40MHz;
   output clock_1MHz,clock_100KHz,clock_10KHz,clock_1KHz,clock_100Hz,clock_10Hz,clock_1Hz;
   reg clock_1MHz,clock_100KHz,clock_10KHz,clock_1KHz,clock_100Hz,clock_10Hz,clock_1Hz;

   reg[4:0] count_1MHz; 
   reg[2:0] count_100KHz,count_10KHz,count_1KHz,count_100Hz,count_10Hz,count_1Hz; 
   reg clock_1MHz_int,clock_100KHz_int,clock_10KHz_int,clock_1KHz_int,clock_100Hz_int,clock_10Hz_int,clock_1Hz_int; 
   // Generate 1Mhz clock
   always @(posedge clock_40MHz)
   begin 
      if (count_1MHz !=19)
         count_1MHz <= count_1MHz + 1 ; 
      else
      begin
         count_1MHz <= 4'b0000 ; 
         clock_1MHz_int <= ~clock_1MHz_int ; 
      end 
      // Ripple clocks are used in this code to save prescalar hardware
      // Sync all clock prescalar outputs back to master clock signal
      clock_1MHz <= clock_1MHz_int ; 
      clock_100KHz <= clock_100KHz_int ; 
      clock_10KHz <= clock_10KHz_int ; 
      clock_1KHz <= clock_1KHz_int ; 
      clock_100Hz <= clock_100Hz_int ; 
      clock_10Hz <= clock_10Hz_int ; 
      clock_1Hz <= clock_1Hz_int ; 
   end 
   // Generate 100Khz clock
   always @(posedge clock_1MHz_int)
   begin 
      if (count_100KHz != 4)
         count_100KHz <= count_100KHz + 1 ; 
      else
      begin
         count_100KHz <= 3'b000 ; 
         clock_100KHz_int <= ~clock_100KHz_int ; 
      end 
   end
   // Generate 10Khz clock 
   always @(posedge clock_100KHz_int)
   begin 
      if (count_10KHz != 4)
         count_10KHz <= count_10KHz + 1 ; 
      else
      begin
         count_10KHz <= 3'b000 ; 
         clock_10KHz_int <= ~clock_10KHz_int ; 
      end 
   end 
   // Generate 1Khz clock
   always @(posedge clock_10KHz_int)
   begin
      if (count_1KHz != 4)
         count_1KHz <= count_1KHz + 1 ; 
      else
      begin
         count_1KHz <= 3'b000 ; 
         clock_1KHz_int <= ~clock_1KHz_int ; 
      end 
   end 
   // Generate 100Hz clock
   always @(posedge clock_1KHz_int)  
   begin
      if (count_100Hz != 4)
         count_100Hz <= count_100Hz + 1 ; 
      else
      begin
         count_100Hz <= 3'b000 ; 
         clock_100Hz_int <= ~clock_100Hz_int ; 
      end 
   end 
   // Generate 10Hz clock
   always @(posedge clock_100Hz_int)
   begin
      if (count_10Hz != 4)
         count_10Hz <= count_10Hz + 1 ; 
      else
      begin
         count_10Hz <= 3'b000 ; 
         clock_10Hz_int <= ~clock_10Hz_int ; 
      end 
   end 
   // Generate 1Hz clock
   always @(posedge clock_10Hz_int)
   begin
      if (count_1Hz != 4)
         count_1Hz <= count_1Hz + 1 ; 
      else
      begin
         count_1Hz <= 3'b000 ; 
         clock_1Hz_int <= ~clock_1Hz_int ; 
      end 
   end 
endmodule

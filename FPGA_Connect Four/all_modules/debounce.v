// Debounce Pushbutton: Filters out mechanical switch bounce for around 40ms.
module debounce (pb, clock_100Hz, pb_debounced);

   input pb; 
   input clock_100Hz; 
   output pb_debounced; 
   //declare the outputs
   reg pb_debounced;
   //declare the internal registers
   reg[3:0] SHIFT_PB; 
   //Debounce clock should be approximately 10ms or 100Hz
   always  @(posedge clock_100Hz)  
   begin
      //Use a shift register to filter switch contact bounce
      SHIFT_PB[2:0] <= SHIFT_PB[3:1] ; 
      SHIFT_PB[3] <= ~pb ; 
      if (SHIFT_PB[3:0] == 4'b0000)
      begin
         pb_debounced <= 1'b0 ; 
      end
      else
      begin
         pb_debounced <= 1'b1 ; 
      end 
   end 
endmodule

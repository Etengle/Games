// Single Pulse circuit 
// the output will go high for only one clock cycle
module onepulse (PB_debounced, clock, PB_single_pulse);

   input PB_debounced; 
   input clock; 
   output PB_single_pulse; 
   //declare the outputs
   reg PB_single_pulse;
   //declare the internal registers
   reg PB_debounced_delay; 
   reg Power_on; 
   //start the process	
   always @(posedge clock)
   begin
      //Power_on will be initialized to 0 (default value) by Xilinx tool
      if (Power_on == 1'b0)
      begin
      	 //This code resets the critical signals once at power up
         PB_single_pulse <= 1'b0 ; 
         PB_debounced_delay <= 1'b1 ; 
         Power_on <= 1'b1 ; 
      end
      else
      begin
      	//A single clock cycle pulse is produced when the switch is hit
	//no matter how long the switch is held down.
	//The switch input must already be debounced.
         if (PB_debounced == 1'b1 & PB_debounced_delay == 1'b0)
            PB_single_pulse <= 1'b1 ; 
         else
            PB_single_pulse <= 1'b0 ; 
            
         PB_debounced_delay <= PB_debounced ; 
      end 
   end 
endmodule

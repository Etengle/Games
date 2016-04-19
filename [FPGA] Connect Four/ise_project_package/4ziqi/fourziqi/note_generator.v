// To generate audio left and audio right at desired note frequency
module note_generator(
  clk, // clock from crystal
  rst_n, // active low reset
  note_div, // divisor for note generation
  audio_left, // left sound audio
  audio_right // right sound audio
);

// I/O declaration
input clk; // clock from crystal
input rst_n; // active low reset
input [19:0] note_div; // divisor for note generation
output [15:0] audio_left; // left sound audio
output [15:0] audio_right; // right sound audio

// Declare internal signals
reg [19:0] clk_cnt_next, clk_cnt;
reg note_clk, note_clk_next;
 
// Note frequency generation
always @(posedge clk or negedge rst_n)
  if (~rst_n)
  begin
    clk_cnt <= 20'd0;
	 note_clk <= 1'b0;
  end
  else
  begin
    clk_cnt <= clk_cnt_next;
	 note_clk <= note_clk_next;
  end
	 
always @* 
  if (clk_cnt == note_div)
  begin
    clk_cnt_next = 20'd0;
	 note_clk_next = ~note_clk;
  end
  else
  begin
    clk_cnt_next = clk_cnt + 1'b1;
	 note_clk_next = note_clk;
  end

  

// We generate two identical square wave signals for the left and right channels
assign audio_left = (note_clk == 1'b0) ? 16'h8000 : 16'h7FFF;
assign audio_right = (note_clk == 1'b0) ? 16'h8000 : 16'h7FFF;

endmodule

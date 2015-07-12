// Generate the five control signals so that the buzzer may produce sound of a specific frequence.
// In version 2, the frequencies of audio_bck (10MHz) and audio_ws (5/32) differ by 64 times.
module buzzer_control(
  clk,  // clock from the crystal
  rst_n,  // active low reset
  audio_in_left, // left channel audio data input
  audio_in_right, // right channel audio data input
  audio_appsel, // playing mode selection
  audio_sysclk, // control clock for DAC (from crystal)
  audio_bck, // bit clock of audio data (10MHz)
  audio_ws, // left/right parallel to serial control
  audio_data // serial output audio data
);

// I/O declaration
input clk;  // clock from the crystal
input rst_n;  // active low reset
input [15:0] audio_in_left; // left channel audio data input
input [15:0] audio_in_right; // right channel audio data input
output audio_appsel; // playing mode selection
output audio_sysclk; // control clock for DAC (40MHz from crystal)
output audio_bck; // bit clock of audio data (10MHz)
output audio_ws; // left/right parallel to serial control
output audio_data; // serial output audio data
reg audio_data;

// Declare internal signal nodes 
wire [7:0] clk_cnt_next;
reg [7:0] clk_cnt;
reg [15:0] audio_left, audio_right;

// Set the audio controller stereo output
assign audio_appsel = 1'b1;

// Counter for the clock divider
assign clk_cnt_next = clk_cnt + 1'b1;

always @(posedge clk or negedge rst_n)
  if (~rst_n)
    clk_cnt <= 8'd0;
  else
    clk_cnt <= clk_cnt_next;

// Assign divided clock output
assign audio_bck = clk_cnt[1];
assign audio_ws = clk_cnt[7];
assign audio_sysclk = clk;

// audio input data buffer
always @(posedge audio_bck or negedge rst_n)
  if (~rst_n)
  begin
    audio_left <= 16'd0;
	 audio_right <= 16'd0;
  end
  else
  begin
    audio_left <= audio_in_left;
	 audio_right <= audio_in_right;
  end

always @(*)
  case (clk_cnt[7:3])
   	 5'b00000: audio_data = audio_right[15];
	 5'b00001: audio_data = audio_right[14];
	 5'b00010: audio_data = audio_right[13];
	 5'b00011: audio_data = audio_right[12];
	 5'b00100: audio_data = audio_right[11];
	 5'b00101: audio_data = audio_right[10];
	 5'b00110: audio_data = audio_right[9];
	 5'b00111: audio_data = audio_right[8];
	 5'b01000: audio_data = audio_right[7];
	 5'b01001: audio_data = audio_right[6];
	 5'b01010: audio_data = audio_right[5];
	 5'b01011: audio_data = audio_right[4];
	 5'b01100: audio_data = audio_right[3];
	 5'b01101: audio_data = audio_right[2];
	 5'b01110: audio_data = audio_right[1];
	 5'b01111: audio_data = audio_right[0];
	 5'b10000: audio_data = audio_left[15];
	 5'b10001: audio_data = audio_left[14];
	 5'b10010: audio_data = audio_left[13];
	 5'b10011: audio_data = audio_left[12];
	 5'b10100: audio_data = audio_left[11];
	 5'b10101: audio_data = audio_left[10];
	 5'b10110: audio_data = audio_left[9];
	 5'b10111: audio_data = audio_left[8];
	 5'b11000: audio_data = audio_left[7];
	 5'b11001: audio_data = audio_left[6];
	 5'b11010: audio_data = audio_left[5];
	 5'b11011: audio_data = audio_left[4];
	 5'b11100: audio_data = audio_left[3];
	 5'b11101: audio_data = audio_left[2];
	 5'b11110: audio_data = audio_left[1];
	 5'b11111: audio_data = audio_left[0];
	 default: audio_data = 1'b0;
  endcase

endmodule

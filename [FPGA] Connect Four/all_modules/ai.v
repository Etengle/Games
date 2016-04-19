
//////////////////////////////////////////////////////////////////////////////////
module ai(bottom,in11,in21,in31,in41,in12,in22,in32,in42,in13,in23,in33,in43,score);
input [2:0]bottom;
input [3:0]in11,in21,in31,in41,in12,in22,in32,in42,in13,in23,in33,in43;
/*output reg [15:0] score;
always@(*) begin

	if (bottom==3'b0)
		score=0;
	else if (in11>=3||in21>=3||in31>=3||in41>=3)
		score=16'b0011_0000_0000_0000;
	else if (in12>=3||in22>=3||in32>=3||in42>=3)
		score=16'b0010_0000_0000_0000;
	else if (bottom==3'b001) 
		score=16'b1<<(in11*3)+16'b1<<(in21*3)+16'b1<<(in31*3)+16'b1<<(in41*3)+16'b1<<(in12*3)+16'b1<<(in22*3)+16'b1<<(in32*3)+16'b1<<(in42*3);
	else if (in13>=3||in23>=3||in33>=3||in43>=3)
			score=16'h0000_0001;
	else 
		score=16'b1<<(in11*3)+16'b1<<(in21*3)+16'b1<<(in31*3)+16'b1<<(in41*3)+16'b1<<(in12*3)+16'b1<<(in22*3)+16'b1<<(in32*3)+16'b1<<(in42*3);

end
*/
output [15:0]score;
assign score[14]=bottom?(in11>=3||in21>=3||in31>=3||in41>=3):0;
assign score[13]=bottom?(in12>=3||in22>=3||in32>=3||in42>=3):0;
assign score[10]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42>=8):0;
assign score[9]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==7):0;
assign score[8]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==6):0;
assign score[7]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==5):0;
assign score[6]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==4):0;
assign score[5]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==3):0;
assign score[4]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==2):0;
assign score[3]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==1):0;
assign score[2]=bottom?(in11+in21+in31+in41+in12+in22+in32+in42==0):0;
assign score[1]=bottom?1:0;

endmodule

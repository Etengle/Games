
module max_(sin1,xin1,sin2,xin2,sout,xout );

input [2:0]xin1,xin2;
input [15:0]sin1,sin2;
output [2:0]xout;
output [15:0]sout;

assign sout=sin1>=sin2?sin1:sin2;
assign xout=sin1>=sin2?xin1:xin2;

endmodule

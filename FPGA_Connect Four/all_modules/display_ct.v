module display_ct (clk, bcds, dig, seg);


	input clk;
	input [15:0] bcds;
	output reg [0:3] dig;
	output reg [0:14] seg;	


parameter 
BCD0 = 15'b0000_0011_1100111,
BCD1 = 15'b1001_1111_1111111,
BCD2 = 15'b0010_0100_1111111,
BCD3 = 15'b0000_1100_1111111,
BCD4 = 15'b1001_1000_1111111,
BCD5 = 15'b0100_1000_1111111,
BCD6 = 15'b0100_0000_1111111,
BCD7 = 15'b0001_1111_1111111,
BCD8 = 15'b0000_0000_1111111,
BCD9 = 15'b0000_1000_1111111,
DARK = 15'b1111_1111_1111111,
P = 15'b0011_0000_1111111,
W = 15'b1001_0011_1110101,
I = 15'b0110_1111_1011011,
N = 15'b1001_0011_0111101,
A = 15'b0001_0000_1111111;
parameter DIG0=4'b0111;
parameter DIG1=4'b1011;
parameter DIG2=4'b1101;
parameter DIG3=4'b1110;
 

reg [0:14]BCD [0:15];
initial
begin
BCD[0]=BCD0;
BCD[1]=BCD1;
BCD[2]=BCD2;
BCD[3]=BCD3;
BCD[4]=BCD4;
BCD[5]=BCD5;
BCD[6]=BCD6;
BCD[7]=BCD7;
BCD[8]=BCD8;
BCD[9]=BCD9;
BCD[10]=DARK;
BCD[11]=P;
BCD[12]=W;
BCD[13]=I;
BCD[14]=N;
BCD[15]=A;

end



reg [1:0]st=2'b00;
reg [27:0] bcd;
reg flag = 0;
integer tmp=0, count = 0;
always@(posedge clk)

begin  
	if (bcds[3:0] != 4'd12) begin flag = 0; bcd = {4'd13,4'd14,4'd10,bcds}; end
	st=st+1;
	
  case(st)
  2'b00:
    begin
    dig<=DIG0;
	 tmp=bcd[15:12];
    
    end
  2'b01:
    begin 
    dig<=DIG1;
    tmp=bcd[11:8];
    end  
  2'b10:
    begin
    dig<=DIG2;
    tmp=bcd[7:4];
    end
  2'b11:
    begin
	 dig<=DIG3;
    tmp=bcd[3:0];
    end
  default:
    begin
    dig<=4'b1111;
    tmp=10;//seg<=DARK;
    end
  endcase
  seg=BCD[tmp];
  count = count + 1;
  if (count == 600) begin
		count = 0;
		if (bcds[3:0] == 4'd12) 
			if (!flag) begin bcd = {4'd13,4'd14,4'd10,bcds}; flag = 1; end
			bcd = {bcd[23:0],bcd[27:24]};
  end
		
end 

  
	endmodule

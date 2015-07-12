
module win(a,c);

input [2:0]a;
output [2:0]c;
wire w1,w2;
and (w1,a[0],a[2]) ,(w2,a[0],~a[1]) ,(c[1],a[0],a[1]);
or (c[0],w1,w2);
assign c[2]=1'b0;

endmodule

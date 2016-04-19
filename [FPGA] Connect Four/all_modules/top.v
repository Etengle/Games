module top( LCD_CLK, LCD_DATA, LCD_ENABLE,
       LCD_RW, LCD_RSTN, LCD_CS1, LCD_CS2, LCD_DI,button,seg,dig,D16
		  , audio_appsel, audio_sysclk, audio_bck, audio_ws, audio_data,Cin,Cout,MODE);
	output [0:3]dig;
	output [0:14]seg;
	output audio_appsel, audio_sysclk, audio_bck, audio_ws, audio_data;
	
   input [7:0]button;
	input  LCD_CLK;
	output [7:0]  LCD_DATA;
	output LCD_ENABLE; 
	output LCD_RW;
	output LCD_RSTN;
	output LCD_CS1 ;
	output LCD_CS2 ;
	output LCD_DI;
	wire clk,clk2,clk3;
	 
	output D16;
	
	input MODE;
	 
	clk_div div(.clock_40MHz(LCD_CLK),.clock_100KHz(clk),.clock_100Hz(clk2),.clock_1KHz(clk3), .clock_10Hz(clk_sw));
	wire [7:0]deb;
	wire [7:0]sp;
	debounce db[7:0](.pb(button), .clock_100Hz(clk2), .pb_debounced(deb));
	onepulse one[7:0] (.PB_debounced(deb), .clock(clk), .PB_single_pulse(sp));
	
	input Cin;
	output Cout;
	assign Cout=clk3;
	
	reg mode=1'b1;
	
	wire rst_n2;
	assign rst_n2=Cin&&MODE==mode;
	


	
	reg [5:0]pos=6'b0,pos_next;
	
	reg [1:0]A[63:0];
	reg [1:0]A_next[63:0];
	
	reg player=1'b0,player_next;
	
	reg [2:0]state=init,state_next=init;
	reg [2:0]bottom[7:0];
	reg [2:0]bottom_next[7:0];
	integer i;
	reg rst_n=1'b1,rst_n_next=1'b1; 
	
	// for buzzer
	reg reset_n;
	reg [19:0] note_div;
	wire [15:0] audio_left, audio_right;
	parameter DO = 20'd76336,
				 RE = 20'd68027, 
				 MI = 20'd60606,
				 FA = 20'd57307,
				 SO = 20'd51020,
				 LA = 20'd45455,
				 SI = 20'd40486,
				 DOH = 20'd19084;
	
	reg [7:0]press;
	//reg [7:0] 
	
	parameter init=3'b0,put=3'b1,pause=3'b10,Wait=3'b11, Ring = 3'b100,AI = 3'b101,AIWAIT=3'b110;
	
	wire [2:0] lfin,rtin,upin,dnin,luin,ruin,ldin,rdin;
	wire [2:0] lf,rt,up,dn,lu,ru,ld,rd;
	wire over;
	
		assign lfin[0]=pos_next%8>0?(player_next?A_next[pos_next-1][1]:A_next[pos_next-1][0]):3'b0;
		assign lfin[1]=pos_next%8>1?(player_next?A_next[pos_next-2][1]:A_next[pos_next-2][0]):3'b0;
		assign lfin[2]=pos_next%8>2?(player_next?A_next[pos_next-3][1]:A_next[pos_next-3][0]):3'b0;
		
		assign rtin[0]=pos_next%8<7?(player_next?A_next[pos_next+1][1]:A_next[pos_next+1][0]):3'b0;
		assign rtin[1]=pos_next%8<6?(player_next?A_next[pos_next+2][1]:A_next[pos_next+2][0]):3'b0;
		assign rtin[2]=pos_next%8<5?(player_next?A_next[pos_next+3][1]:A_next[pos_next+3][0]):3'b0;
				
		assign dnin[0]=pos_next<56?(player_next?A_next[pos_next+8][1]:A_next[pos_next+8][0]):3'b0;
		assign dnin[1]=pos_next<48?(player_next?A_next[pos_next+16][1]:A_next[pos_next+16][0]):3'b0;
		assign dnin[2]=pos_next<40?(player_next?A_next[pos_next+24][1]:A_next[pos_next+24][0]):3'b0;
				
		assign upin[0]=pos_next>=8?(player?A_next[pos_next-8][0]:A_next[pos_next-8][1]):3'b0;
		assign upin[1]=pos_next>=16?(player?A_next[pos_next-16][0]:A_next[pos_next-16][1]):3'b0;
		assign upin[2]=pos_next>=24?(player?A_next[pos_next-24][0]:A_next[pos_next-24][1]):3'b0;
		
		assign luin[0]= (pos_next%8>0 && pos_next>=8) ? (player_next?A_next[pos_next-9][1]:A_next[pos_next-9][0]):3'b0;
		assign luin[1]= (pos_next%8>1 && pos_next>=16) ? (player_next?A_next[pos_next-18][1]:A_next[pos_next-18][0]):3'b0;
		assign luin[2]= (pos_next%8>2 && pos_next>=24) ? (player_next?A_next[pos_next-27][1]:A_next[pos_next-27][0]):3'b0;
		
		assign ldin[0]= (pos_next%8>0 && pos_next<56) ? (player_next?A_next[pos_next+7][1]:A_next[pos_next+7][0]):3'b0;
		assign ldin[1]= (pos_next%8>1 && pos_next<48) ? (player_next?A_next[pos_next+14][1]:A_next[pos_next+14][0]):3'b0;
		assign ldin[2]= (pos_next%8>2 && pos_next<40) ? (player_next?A_next[pos_next+21][1]:A_next[pos_next+21][0]):3'b0;
		
		assign ruin[0]= (pos_next%8<7 && pos_next>=8) ? (player_next?A_next[pos_next-7][1]:A_next[pos_next-7][0]):3'b0;
		assign ruin[1]= (pos_next%8<6 && pos_next>=16) ? (player_next?A_next[pos_next-14][1]:A_next[pos_next-14][0]):3'b0;
		assign ruin[2]= (pos_next%8<5 && pos_next>=24) ? (player_next?A_next[pos_next-21][1]:A_next[pos_next-21][0]):3'b0;
		
		assign rdin[0]= (pos_next%8<7 && pos_next<56) ? (player_next?A_next[pos_next+9][1]:A_next[pos_next+9][0]):3'b0;
		assign rdin[1]= (pos_next%8<6 && pos_next<48) ? (player_next?A_next[pos_next+18][1]:A_next[pos_next+18][0]):3'b0;
		assign rdin[2]= (pos_next%8<5 && pos_next<40) ? (player_next?A_next[pos_next+27][1]:A_next[pos_next+27][0]):3'b0;
		
	win 
	w1 (.a(lfin),.c(lf)),
	w2 (.a(rtin),.c(rt)),
	w3 (.a(upin),.c(up)),
	w4 (.a(dnin),.c(dn)),
	w5 (.a(luin),.c(lu)),
	w6 (.a(ruin),.c(ru)),
	w7 (.a(ldin),.c(ld)),
	w8 (.a(rdin),.c(rd));
	
	assign over=(lf+rt>=3)||(up+dn>=3)||(lu+rd>=3)||(ld+ru>=3);
	wire [3:0] lr,ud,lurd,ldru;
	assign lr=lf+rt;
	assign ud=up+dn;
	assign lurd=lu+rd;
	assign ldru=ld+ru;
	/*-------------------------------------------AI TOP-------------------------------------------------------------------*/
	
	
	reg [2:0] 
	lfin1[7:0],rtin1[7:0],dnin1[7:0],luin1[7:0],ruin1[7:0],ldin1[7:0],rdin1[7:0],
	lfin2[7:0],rtin2[7:0],dnin2[7:0],luin2[7:0],ruin2[7:0],ldin2[7:0],rdin2[7:0],
	lfin3[7:0],rtin3[7:0],dnin3[7:0],luin3[7:0],ruin3[7:0],ldin3[7:0],rdin3[7:0];
	
	wire [2:0] 
	lf1[7:0],rt1[7:0],dn1[7:0],lu1[7:0],ru1[7:0],ld1[7:0],rd1[7:0],
	lf2[7:0],rt2[7:0],dn2[7:0],lu2[7:0],ru2[7:0],ld2[7:0],rd2[7:0],
	lf3[7:0],rt3[7:0],dn3[7:0],lu3[7:0],ru3[7:0],ld3[7:0],rd3[7:0];
	reg [5:0]ai_pos[7:0];
	integer k;
	always@(*) begin
		for(k=0;k<=7;k=k+1) begin
			ai_pos[k]=bottom[k]*8+k;
			/*in1*/
			lfin1[k][0]=ai_pos[k]%8>0?A[ai_pos[k]-1][0]:3'b0;
			lfin1[k][1]=ai_pos[k]%8>1?A[ai_pos[k]-2][0]:3'b0;
			lfin1[k][2]=ai_pos[k]%8>2?(A[ai_pos[k]-3][0]):3'b0;
		
			rtin1[k][0]=ai_pos[k]%8<7?(A[ai_pos[k]+1][0]):3'b0;
			rtin1[k][1]=ai_pos[k]%8<6?(A[ai_pos[k]+2][0]):3'b0;
			rtin1[k][2]=ai_pos[k]%8<5?(A[ai_pos[k]+3][0]):3'b0;
				
			dnin1[k][0]=ai_pos[k]<56?(A[ai_pos[k]+8][0]):3'b0;
			dnin1[k][1]=ai_pos[k]<48?(A[ai_pos[k]+16][0]):3'b0;
			dnin1[k][2]=ai_pos[k]<40?(A[ai_pos[k]+24][0]):3'b0;
				
			
		
			luin1[k][0]= (ai_pos[k]%8>0 && ai_pos[k]>=8) ? (A[ai_pos[k]-9][0]):3'b0;
			luin1[k][1]= (ai_pos[k]%8>1 && ai_pos[k]>=16) ? (A[ai_pos[k]-18][0]):3'b0;
			luin1[k][2]= (ai_pos[k]%8>2 && ai_pos[k]>=24) ? (A[ai_pos[k]-27][0]):3'b0;
		
			ldin1[k][0]= (ai_pos[k]%8>0 && ai_pos[k]<56) ? (A[ai_pos[k]+7][0]):3'b0;
			ldin1[k][1]= (ai_pos[k]%8>1 && ai_pos[k]<48) ? (A[ai_pos[k]+14][0]):3'b0;
			ldin1[k][2]= (ai_pos[k]%8>2 && ai_pos[k]<40) ? (A[ai_pos[k]+21][0]):3'b0;
		
			ruin1[k][0]= (ai_pos[k]%8<7 && ai_pos[k]>=8) ? (A[ai_pos[k]-7][0]):3'b0;
			ruin1[k][1]= (ai_pos[k]%8<6 && ai_pos[k]>=16) ? (A[ai_pos[k]-14][0]):3'b0;
			ruin1[k][2]= (ai_pos[k]%8<5 && ai_pos[k]>=24) ? (A[ai_pos[k]-21][0]):3'b0;
		
			rdin1[k][0]= (ai_pos[k]%8<7 && ai_pos[k]<56) ? (A[ai_pos[k]+9][0]):3'b0;
			rdin1[k][1]= (ai_pos[k]%8<6 && ai_pos[k]<48) ? (A[ai_pos[k]+18][0]):3'b0;
			rdin1[k][2]= (ai_pos[k]%8<5 && ai_pos[k]<40) ? (A[ai_pos[k]+27][0]):3'b0;
			/*in2*/
			lfin2[k][0]=ai_pos[k]%8>0?(A[ai_pos[k]-1][1]):3'b0;
			lfin2[k][1]=ai_pos[k]%8>1?(A[ai_pos[k]-2][1]):3'b0;
			lfin2[k][2]=ai_pos[k]%8>2?(A[ai_pos[k]-3][1]):3'b0;
		
			rtin2[k][0]=ai_pos[k]%8<7?(A[ai_pos[k]+1][1]):3'b0;
			rtin2[k][1]=ai_pos[k]%8<6?(A[ai_pos[k]+2][1]):3'b0;
			rtin2[k][2]=ai_pos[k]%8<5?(A[ai_pos[k]+3][1]):3'b0;
				
			dnin2[k][0]=ai_pos[k]<56?(A[ai_pos[k]+8][1]):3'b0;
			dnin2[k][1]=ai_pos[k]<48?(A[ai_pos[k]+16][1]):3'b0;
			dnin2[k][2]=ai_pos[k]<40?(A[ai_pos[k]+24][1]):3'b0;
				
		
		
			luin2[k][0]= (ai_pos[k]%8>0 && ai_pos[k]>=8) ? (A[ai_pos[k]-9][1]):3'b0;
			luin2[k][1]= (ai_pos[k]%8>1 && ai_pos[k]>=16) ? (A[ai_pos[k]-18][1]):3'b0;
			luin2[k][2]= (ai_pos[k]%8>2 && ai_pos[k]>=24) ? (A[ai_pos[k]-27][1]):3'b0;
		
			ldin2[k][0]= (ai_pos[k]%8>0 && ai_pos[k]<56) ? (A[ai_pos[k]+7][1]):3'b0;
			ldin2[k][1]= (ai_pos[k]%8>1 && ai_pos[k]<48) ? (A[ai_pos[k]+14][1]):3'b0;
			ldin2[k][2]= (ai_pos[k]%8>2 && ai_pos[k]<40) ? (A[ai_pos[k]+21][1]):3'b0;
		
			ruin2[k][0]= (ai_pos[k]%8<7 && ai_pos[k]>=8) ? (A[ai_pos[k]-7][1]):3'b0;
			ruin2[k][1]= (ai_pos[k]%8<6 && ai_pos[k]>=16) ? (A[ai_pos[k]-14][1]):3'b0;
			ruin2[k][2]= (ai_pos[k]%8<5 && ai_pos[k]>=24) ? (A[ai_pos[k]-21][1]):3'b0;
		
			rdin2[k][0]= (ai_pos[k]%8<7 && ai_pos[k]<56) ? (A[ai_pos[k]+9][1]):3'b0;
			rdin2[k][1]= (ai_pos[k]%8<6 && ai_pos[k]<48) ? (A[ai_pos[k]+18][1]):3'b0;
			rdin2[k][2]= (ai_pos[k]%8<5 && ai_pos[k]<40) ? (A[ai_pos[k]+27][1]):3'b0;

			/*in3*/
		/*	ai_pos[k]=ai_pos[k]-8;
			
			lfin3[k][0]=ai_pos[k]%8>0?(A[ai_pos[k]-1][1]):3'b0;
			lfin3[k][1]=ai_pos[k]%8>1?(A[ai_pos[k]-2][1]):3'b0;
			lfin3[k][2]=ai_pos[k]%8>2?(A[ai_pos[k]-3][1]):3'b0;
		
			rtin3[k][0]=ai_pos[k]%8<7?(A[ai_pos[k]+1][1]):3'b0;
			rtin3[k][1]=ai_pos[k]%8<6?(A[ai_pos[k]+2][1]):3'b0;
			rtin3[k][2]=ai_pos[k]%8<5?(A[ai_pos[k]+3][1]):3'b0;
				
			dnin3[k][0]=ai_pos[k]<56?(A[ai_pos[k]+8][1]):3'b0;
			dnin3[k][1]=ai_pos[k]<48?(A[ai_pos[k]+16][1]):3'b0;
			dnin3[k][2]=ai_pos[k]<40?(A[ai_pos[k]+24][1]):3'b0;
				
	
		
			luin3[k][0]= (ai_pos[k]%8>0 && ai_pos[k]>=8) ? (A[ai_pos[k]-9][1]):3'b0;
			luin3[k][1]= (ai_pos[k]%8>1 && ai_pos[k]>=16) ? (A[ai_pos[k]-18][1]):3'b0;
			luin3[k][2]= (ai_pos[k]%8>2 && ai_pos[k]>=24) ? (A[ai_pos[k]-27][1]):3'b0;
		
			ldin3[k][0]= (ai_pos[k]%8>0 && ai_pos[k]<56) ? (A[ai_pos[k]+7][1]):3'b0;
			ldin3[k][1]= (ai_pos[k]%8>1 && ai_pos[k]<48) ? (A[ai_pos[k]+14][1]):3'b0;
			ldin3[k][2]= (ai_pos[k]%8>2 && ai_pos[k]<40) ? (A[ai_pos[k]+21][1]):3'b0;
	
			ruin3[k][0]= (ai_pos[k]%8<7 && ai_pos[k]>=8) ? (A[ai_pos[k]-7][1]):3'b0;
			ruin3[k][1]= (ai_pos[k]%8<6 && ai_pos[k]>=16) ? (A[ai_pos[k]-14][1]):3'b0;
			ruin3[k][2]= (ai_pos[k]%8<5 && ai_pos[k]>=24) ? (A[ai_pos[k]-21][1]):3'b0;
		
			rdin3[k][0]= (ai_pos[k]%8<7 && ai_pos[k]<56) ? (A[ai_pos[k]+9][1]):3'b0;
			rdin3[k][1]= (ai_pos[k]%8<6 && ai_pos[k]<48) ? (A[ai_pos[k]+18][1]):3'b0;
			rdin3[k][2]= (ai_pos[k]%8<5 && ai_pos[k]<40) ? (A[ai_pos[k]+27][1]):3'b0;

*/
		end
	end
	win
	ai17_w1 (.a(lfin1[7]),.c(lf1[7])),ai16_w1 (.a(lfin1[6]),.c(lf1[6])),ai15_w1 (.a(lfin1[5]),.c(lf1[5])),ai14_w1 (.a(lfin1[4]),.c(lf1[4])),ai13_w1 (.a(lfin1[3]),.c(lf1[3])),ai12_w1 (.a(lfin1[2]),.c(lf1[2])),ai11_w1 (.a(lfin1[1]),.c(lf1[1])),ai10_w1 (.a(lfin1[0]),.c(lf1[0])),
	ai17_w2 (.a(rtin1[7]),.c(rt1[7])),ai16_w2 (.a(rtin1[6]),.c(rt1[6])),ai15_w2 (.a(rtin1[5]),.c(rt1[5])),ai14_w2 (.a(rtin1[4]),.c(rt1[4])),ai13_w2 (.a(rtin1[3]),.c(rt1[3])),ai12_w2 (.a(rtin1[2]),.c(rt1[2])),ai11_w2 (.a(rtin1[1]),.c(rt1[1])),ai10_w2 (.a(rtin1[0]),.c(rt1[0])),
	
	ai17_w4 (.a(dnin1[7]),.c(dn1[7])),ai16_w4 (.a(dnin1[6]),.c(dn1[6])),ai15_w4 (.a(dnin1[5]),.c(dn1[5])),ai14_w4 (.a(dnin1[4]),.c(dn1[4])),ai13_w4 (.a(dnin1[3]),.c(dn1[3])),ai12_w4 (.a(dnin1[2]),.c(dn1[2])),ai11_w4 (.a(dnin1[1]),.c(dn1[1])),ai10_w4 (.a(dnin1[0]),.c(dn1[0])),
	ai17_w5 (.a(luin1[7]),.c(lu1[7])),ai16_w5 (.a(luin1[6]),.c(lu1[6])),ai15_w5 (.a(luin1[5]),.c(lu1[5])),ai14_w5 (.a(luin1[4]),.c(lu1[4])),ai13_w5 (.a(luin1[3]),.c(lu1[3])),ai12_w5 (.a(luin1[2]),.c(lu1[2])),ai11_w5 (.a(luin1[1]),.c(lu1[1])),ai10_w5 (.a(luin1[0]),.c(lu1[0])),
	ai17_w6 (.a(ruin1[7]),.c(ru1[7])),ai16_w6 (.a(ruin1[6]),.c(ru1[6])),ai15_w6 (.a(ruin1[5]),.c(ru1[5])),ai14_w6 (.a(ruin1[4]),.c(ru1[4])),ai13_w6 (.a(ruin1[3]),.c(ru1[3])),ai12_w6 (.a(ruin1[2]),.c(ru1[2])),ai11_w6 (.a(ruin1[1]),.c(ru1[1])),ai10_w6 (.a(ruin1[0]),.c(ru1[0])),
	ai17_w7 (.a(ldin1[7]),.c(ld1[7])),ai16_w7 (.a(ldin1[6]),.c(ld1[6])),ai15_w7 (.a(ldin1[5]),.c(ld1[5])),ai14_w7 (.a(ldin1[4]),.c(ld1[4])),ai13_w7 (.a(ldin1[3]),.c(ld1[3])),ai12_w7 (.a(ldin1[2]),.c(ld1[2])),ai11_w7 (.a(ldin1[1]),.c(ld1[1])),ai10_w7 (.a(ldin1[0]),.c(ld1[0])),
	ai17_w8 (.a(rdin1[7]),.c(rd1[7])),ai16_w8 (.a(rdin1[6]),.c(rd1[6])),ai15_w8 (.a(rdin1[5]),.c(rd1[5])),ai14_w8 (.a(rdin1[4]),.c(rd1[4])),ai13_w8 (.a(rdin1[3]),.c(rd1[3])),ai12_w8 (.a(rdin1[2]),.c(rd1[2])),ai11_w8 (.a(rdin1[1]),.c(rd1[1])),ai10_w8 (.a(rdin1[0]),.c(rd1[0])),
	
	ai27_w1 (.a(lfin2[7]),.c(lf2[7])),ai26_w1 (.a(lfin2[6]),.c(lf2[6])),ai25_w1 (.a(lfin2[5]),.c(lf2[5])),ai24_w1 (.a(lfin2[4]),.c(lf2[4])),ai23_w1 (.a(lfin2[3]),.c(lf2[3])),ai22_w1 (.a(lfin2[2]),.c(lf2[2])),ai21_w1 (.a(lfin2[1]),.c(lf2[1])),ai20_w1 (.a(lfin2[0]),.c(lf2[0])),
	ai27_w2 (.a(rtin2[7]),.c(rt2[7])),ai26_w2 (.a(rtin2[6]),.c(rt2[6])),ai25_w2 (.a(rtin2[5]),.c(rt2[5])),ai24_w2 (.a(rtin2[4]),.c(rt2[4])),ai23_w2 (.a(rtin2[3]),.c(rt2[3])),ai22_w2 (.a(rtin2[2]),.c(rt2[2])),ai21_w2 (.a(rtin2[1]),.c(rt2[1])),ai20_w2 (.a(rtin2[0]),.c(rt2[0])),
	
	ai27_w4 (.a(dnin2[7]),.c(dn2[7])),ai26_w4 (.a(dnin2[6]),.c(dn2[6])),ai25_w4 (.a(dnin2[5]),.c(dn2[5])),ai24_w4 (.a(dnin2[4]),.c(dn2[4])),ai23_w4 (.a(dnin2[3]),.c(dn2[3])),ai22_w4 (.a(dnin2[2]),.c(dn2[2])),ai21_w4 (.a(dnin2[1]),.c(dn2[1])),ai20_w4 (.a(dnin2[0]),.c(dn2[0])),
	ai27_w5 (.a(luin2[7]),.c(lu2[7])),ai26_w5 (.a(luin2[6]),.c(lu2[6])),ai25_w5 (.a(luin2[5]),.c(lu2[5])),ai24_w5 (.a(luin2[4]),.c(lu2[4])),ai23_w5 (.a(luin2[3]),.c(lu2[3])),ai22_w5 (.a(luin2[2]),.c(lu2[2])),ai21_w5 (.a(luin2[1]),.c(lu2[1])),ai20_w5 (.a(luin2[0]),.c(lu2[0])),
	ai27_w6 (.a(ruin2[7]),.c(ru2[7])),ai26_w6 (.a(ruin2[6]),.c(ru2[6])),ai25_w6 (.a(ruin2[5]),.c(ru2[5])),ai24_w6 (.a(ruin2[4]),.c(ru2[4])),ai23_w6 (.a(ruin2[3]),.c(ru2[3])),ai22_w6 (.a(ruin2[2]),.c(ru2[2])),ai21_w6 (.a(ruin2[1]),.c(ru2[1])),ai20_w6 (.a(ruin2[0]),.c(ru2[0])),
	ai27_w7 (.a(ldin2[7]),.c(ld2[7])),ai26_w7 (.a(ldin2[6]),.c(ld2[6])),ai25_w7 (.a(ldin2[5]),.c(ld2[5])),ai24_w7 (.a(ldin2[4]),.c(ld2[4])),ai23_w7 (.a(ldin2[3]),.c(ld2[3])),ai22_w7 (.a(ldin2[2]),.c(ld2[2])),ai21_w7 (.a(ldin2[1]),.c(ld2[1])),ai20_w7 (.a(ldin2[0]),.c(ld2[0])),
	ai27_w8 (.a(rdin2[7]),.c(rd2[7])),ai26_w8 (.a(rdin2[6]),.c(rd2[6])),ai25_w8 (.a(rdin2[5]),.c(rd2[5])),ai24_w8 (.a(rdin2[4]),.c(rd2[4])),ai23_w8 (.a(rdin2[3]),.c(rd2[3])),ai22_w8 (.a(rdin2[2]),.c(rd2[2])),ai21_w8 (.a(rdin2[1]),.c(rd2[1])),ai20_w8 (.a(rdin2[0]),.c(rd2[0]));
/*	
	ai37_w1 (.a(lfin3[7]),.c(lf3[7])),ai36_w1 (.a(lfin3[6]),.c(lf3[6])),ai35_w1 (.a(lfin3[5]),.c(lf3[5])),ai34_w1 (.a(lfin3[4]),.c(lf3[4])),ai33_w1 (.a(lfin3[3]),.c(lf3[3])),ai32_w1 (.a(lfin3[2]),.c(lf3[2])),ai31_w1 (.a(lfin3[1]),.c(lf3[1])),ai30_w1 (.a(lfin3[0]),.c(lf3[0])),
	ai37_w2 (.a(rtin3[7]),.c(rt3[7])),ai36_w2 (.a(rtin3[6]),.c(rt3[6])),ai35_w2 (.a(rtin3[5]),.c(rt3[5])),ai34_w2 (.a(rtin3[4]),.c(rt3[4])),ai33_w2 (.a(rtin3[3]),.c(rt3[3])),ai32_w2 (.a(rtin3[2]),.c(rt3[2])),ai31_w2 (.a(rtin3[1]),.c(rt3[1])),ai30_w2 (.a(rtin3[0]),.c(rt3[0])),
	
	ai37_w4 (.a(dnin3[7]),.c(dn3[7])),ai36_w4 (.a(dnin3[6]),.c(dn3[6])),ai35_w4 (.a(dnin3[5]),.c(dn3[5])),ai34_w4 (.a(dnin3[4]),.c(dn3[4])),ai33_w4 (.a(dnin3[3]),.c(dn3[3])),ai32_w4 (.a(dnin3[2]),.c(dn3[2])),ai31_w4 (.a(dnin3[1]),.c(dn3[1])),ai30_w4 (.a(dnin3[0]),.c(dn3[0])),
	ai37_w5 (.a(luin3[7]),.c(lu3[7])),ai36_w5 (.a(luin3[6]),.c(lu3[6])),ai35_w5 (.a(luin3[5]),.c(lu3[5])),ai34_w5 (.a(luin3[4]),.c(lu3[4])),ai33_w5 (.a(luin3[3]),.c(lu3[3])),ai32_w5 (.a(luin3[2]),.c(lu3[2])),ai31_w5 (.a(luin3[1]),.c(lu3[1])),ai30_w5 (.a(luin3[0]),.c(lu3[0])),
	ai37_w6 (.a(ruin3[7]),.c(ru3[7])),ai36_w6 (.a(ruin3[6]),.c(ru3[6])),ai35_w6 (.a(ruin3[5]),.c(ru3[5])),ai34_w6 (.a(ruin3[4]),.c(ru3[4])),ai33_w6 (.a(ruin3[3]),.c(ru3[3])),ai32_w6 (.a(ruin3[2]),.c(ru3[2])),ai31_w6 (.a(ruin3[1]),.c(ru3[1])),ai30_w6 (.a(ruin3[0]),.c(ru3[0])),
	ai37_w7 (.a(ldin3[7]),.c(ld3[7])),ai36_w7 (.a(ldin3[6]),.c(ld3[6])),ai35_w7 (.a(ldin3[5]),.c(ld3[5])),ai34_w7 (.a(ldin3[4]),.c(ld3[4])),ai33_w7 (.a(ldin3[3]),.c(ld3[3])),ai32_w7 (.a(ldin3[2]),.c(ld3[2])),ai31_w7 (.a(ldin3[1]),.c(ld3[1])),ai30_w7 (.a(ldin3[0]),.c(ld3[0])),
	ai37_w8 (.a(rdin3[7]),.c(rd3[7])),ai36_w8 (.a(rdin3[6]),.c(rd3[6])),ai35_w8 (.a(rdin3[5]),.c(rd3[5])),ai34_w8 (.a(rdin3[4]),.c(rd3[4])),ai33_w8 (.a(rdin3[3]),.c(rd3[3])),ai32_w8 (.a(rdin3[2]),.c(rd3[2])),ai31_w8 (.a(rdin3[1]),.c(rd3[1])),ai30_w8 (.a(rdin3[0]),.c(rd3[0]));
	*/
	wire [15:0] score[7:0]; 
	
/*	ai ai7(.bottom(bottom[7]),.in11(lf1[7]+rt1[7]),.in21(dn1[7]),.in31(lu1[7]+rd1[7]),.in41(ru1[7]+ld1[7]),.in12(lf2[7]+rt2[7]),.in22(dn2[7]),.in32(lu2[7]+rd2[7]),.in42(ru2[7]+ld2[7]),.in13(lf3[7]+rt3[7]),.in23(dn3[7]),.in33(lu3[7]+rd3[7]),.in43(ru3[7]+ld3[7]),.score(score[7]));
	ai ai6(.bottom(bottom[6]),.in11(lf1[6]+rt1[6]),.in21(dn1[6]),.in31(lu1[6]+rd1[6]),.in41(ru1[6]+ld1[6]),.in12(lf2[6]+rt2[6]),.in22(dn2[6]),.in32(lu2[6]+rd2[6]),.in42(ru2[6]+ld2[6]),.in13(lf3[6]+rt3[6]),.in23(dn3[6]),.in33(lu3[6]+rd3[6]),.in43(ru3[6]+ld3[6]),.score(score[6]));
	ai ai5(.bottom(bottom[5]),.in11(lf1[5]+rt1[5]),.in21(dn1[5]),.in31(lu1[5]+rd1[5]),.in41(ru1[5]+ld1[5]),.in12(lf2[5]+rt2[5]),.in22(dn2[5]),.in32(lu2[5]+rd2[5]),.in42(ru2[5]+ld2[5]),.in13(lf3[5]+rt3[5]),.in23(dn3[5]),.in33(lu3[5]+rd3[5]),.in43(ru3[5]+ld3[5]),.score(score[5]));
	ai ai4(.bottom(bottom[4]),.in11(lf1[4]+rt1[4]),.in21(dn1[4]),.in31(lu1[4]+rd1[4]),.in41(ru1[4]+ld1[4]),.in12(lf2[4]+rt2[4]),.in22(dn2[4]),.in32(lu2[4]+rd2[4]),.in42(ru2[4]+ld2[4]),.in13(lf3[4]+rt3[4]),.in23(dn3[4]),.in33(lu3[4]+rd3[4]),.in43(ru3[4]+ld3[4]),.score(score[4]));
	ai ai3(.bottom(bottom[3]),.in11(lf1[3]+rt1[3]),.in21(dn1[3]),.in31(lu1[3]+rd1[3]),.in41(ru1[3]+ld1[3]),.in12(lf2[3]+rt2[3]),.in22(dn2[3]),.in32(lu2[3]+rd2[3]),.in42(ru2[3]+ld2[3]),.in13(lf3[3]+rt3[3]),.in23(dn3[3]),.in33(lu3[3]+rd3[3]),.in43(ru3[3]+ld3[3]),.score(score[3]));
	ai ai2(.bottom(bottom[2]),.in11(lf1[2]+rt1[2]),.in21(dn1[2]),.in31(lu1[2]+rd1[2]),.in41(ru1[2]+ld1[2]),.in12(lf2[2]+rt2[2]),.in22(dn2[2]),.in32(lu2[2]+rd2[2]),.in42(ru2[2]+ld2[2]),.in13(lf3[2]+rt3[2]),.in23(dn3[2]),.in33(lu3[2]+rd3[2]),.in43(ru3[2]+ld3[2]),.score(score[2]));
	ai ai1(.bottom(bottom[1]),.in11(lf1[1]+rt1[1]),.in21(dn1[1]),.in31(lu1[1]+rd1[1]),.in41(ru1[1]+ld1[1]),.in12(lf2[1]+rt2[1]),.in22(dn2[1]),.in32(lu2[1]+rd2[1]),.in42(ru2[1]+ld2[1]),.in13(lf3[1]+rt3[1]),.in23(dn3[1]),.in33(lu3[1]+rd3[1]),.in43(ru3[1]+ld3[1]),.score(score[1]));
	ai ai0(.bottom(bottom[0]),.in11(lf1[0]+rt1[0]),.in21(dn1[0]),.in31(lu1[0]+rd1[0]),.in41(ru1[0]+ld1[0]),.in12(lf2[0]+rt2[0]),.in22(dn2[0]),.in32(lu2[0]+rd2[0]),.in42(ru2[0]+ld2[0]),.in13(lf3[0]+rt3[0]),.in23(dn3[0]),.in33(lu3[0]+rd3[0]),.in43(ru3[0]+ld3[0]),.score(score[0]));
	*/
	
	ai ai7(.bottom(bottom[7]),.in11(lf1[7]+rt1[7]),.in21(dn1[7]),.in31(lu1[7]+rd1[7]),.in41(ru1[7]+ld1[7]),.in12(lf2[7]+rt2[7]),.in22(dn2[7]),.in32(lu2[7]+rd2[7]),.in42(ru2[7]+ld2[7]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[7]));
	ai ai6(.bottom(bottom[6]),.in11(lf1[6]+rt1[6]),.in21(dn1[6]),.in31(lu1[6]+rd1[6]),.in41(ru1[6]+ld1[6]),.in12(lf2[6]+rt2[6]),.in22(dn2[6]),.in32(lu2[6]+rd2[6]),.in42(ru2[6]+ld2[6]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[6]));
	ai ai5(.bottom(bottom[5]),.in11(lf1[5]+rt1[5]),.in21(dn1[5]),.in31(lu1[5]+rd1[5]),.in41(ru1[5]+ld1[5]),.in12(lf2[5]+rt2[5]),.in22(dn2[5]),.in32(lu2[5]+rd2[5]),.in42(ru2[5]+ld2[5]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[5]));
	ai ai4(.bottom(bottom[4]),.in11(lf1[4]+rt1[4]),.in21(dn1[4]),.in31(lu1[4]+rd1[4]),.in41(ru1[4]+ld1[4]),.in12(lf2[4]+rt2[4]),.in22(dn2[4]),.in32(lu2[4]+rd2[4]),.in42(ru2[4]+ld2[4]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[4]));
	ai ai3(.bottom(bottom[3]),.in11(lf1[3]+rt1[3]),.in21(dn1[3]),.in31(lu1[3]+rd1[3]),.in41(ru1[3]+ld1[3]),.in12(lf2[3]+rt2[3]),.in22(dn2[3]),.in32(lu2[3]+rd2[3]),.in42(ru2[3]+ld2[3]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[3]));
	ai ai2(.bottom(bottom[2]),.in11(lf1[2]+rt1[2]),.in21(dn1[2]),.in31(lu1[2]+rd1[2]),.in41(ru1[2]+ld1[2]),.in12(lf2[2]+rt2[2]),.in22(dn2[2]),.in32(lu2[2]+rd2[2]),.in42(ru2[2]+ld2[2]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[2]));
	ai ai1(.bottom(bottom[1]),.in11(lf1[1]+rt1[1]),.in21(dn1[1]),.in31(lu1[1]+rd1[1]),.in41(ru1[1]+ld1[1]),.in12(lf2[1]+rt2[1]),.in22(dn2[1]),.in32(lu2[1]+rd2[1]),.in42(ru2[1]+ld2[1]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[1]));
	ai ai0(.bottom(bottom[0]),.in11(lf1[0]+rt1[0]),.in21(dn1[0]),.in31(lu1[0]+rd1[0]),.in41(ru1[0]+ld1[0]),.in12(lf2[0]+rt2[0]),.in22(dn2[0]),.in32(lu2[0]+rd2[0]),.in42(ru2[0]+ld2[0]),.in13(0),.in23(0),.in33(0),.in43(0),.score(score[0]));
	
	
wire [2:0]  x01, x23, x45, x67, x03, x47, x07;
wire [15:0] s01, s23, s45, s67, s03, s47, s07;	
	
	max_ 
	m01(.sin1(score[0]),.xin1(3'b000),.sin2(score[1]),.xin2(3'b001),.sout(s01),.xout(x01)),
	m23(.sin1(score[2]),.xin1(3'b010),.sin2(score[3]),.xin2(3'b011),.sout(s23),.xout(x23)),
	m45(.sin1(score[4]),.xin1(3'b100),.sin2(score[5]),.xin2(3'b101),.sout(s45),.xout(x45)),
	m67(.sin1(score[6]),.xin1(3'b110),.sin2(score[7]),.xin2(3'b111),.sout(s67),.xout(x67)),
	
	m03(.sin1(s01),.xin1(x01),.sin2(s23),.xin2(x23),.sout(s03),.xout(x03)),
	m47(.sin1(s45),.xin1(x45),.sin2(s67),.xin2(x67),.sout(s47),.xout(x47)),
	
	m07(.sin1(s03),.xin1(x03),.sin2(s47),.xin2(x47),.sout(s07),.xout(x07));
	
  	wire [5:0]decide;
	assign decide=x07;
	/*-------------------------------------------AI BOTTOM--------------------------------------------------------------**/
	
	
	wire done;
	integer counter = 0;
	
	reg doing = 0;
	reg ringdone;
	
	parameter delay=0;//17'd100000;
	reg [16:0]wait_count,wait_count_next;
	always@(negedge clk) begin
		if (!rst_n2) begin
			for(i=0;i<=7;i=i+1)
				bottom[i]<=3'b111;
			for(i=0;i<=63;i=i+1)
				A[i]<=2'b00;
			state<=init;
			player<=1'b0;
			press<=8'b0;
			pos<=0;
			rst_n<=1'b1;
			wait_count<=17'b0;
			mode=MODE;
		end
		else if (over) begin
			state<=pause;
			for(i=0;i<=7;i=i+1)
				bottom[i]<=bottom_next[i];
			for(i=0;i<=63;i=i+1)
				A[i]<=A_next[i];
			player<=player_next;
			pos<=pos_next;
			press<=sp;
			rst_n<=rst_n_next;
			wait_count<=wait_count_next;
		end
		else begin
			
			state<=state_next;
			if (!doing) begin
			for(i=0;i<=7;i=i+1)
				bottom[i]<=bottom_next[i];
			for(i=0;i<=63;i=i+1)
				A[i]<=A_next[i];
			player<=player_next;
			press<=sp;
			pos<=pos_next;
			rst_n<=rst_n_next;
			wait_count<=wait_count_next;
			end
		end
	
	end
	reg [1:0] note = 0; 
	always@(negedge clk_sw) begin
		if (!rst_n2) begin
			reset_n = 0; counter = 0; ringdone = 1; note = 0;
			note_div = 0;
		end
		else if (state == Ring) begin
			note_div = player ? DO : DOH;
			reset_n = 1;
			counter = counter + 1;
			if (counter == 2) begin reset_n = 0; counter = 0; ringdone = 1; end
		end
		else if (state == pause && !ringdone) begin
			case (note) 
				2'b0: note_div = DO;
				2'b1: note_div = MI;
				2'b10: note_div = SO;
				2'b11: note_div = DOH;
			endcase
			reset_n = 1;
			counter = counter + 1;
			if (counter == 4) begin 
				reset_n = 0; 
				counter = 0;
				if (note == 2'b11) ringdone = 1;
				note = note + 1; 
			end
		end
		else if (state != pause) ringdone = 0;
	end
	
	
	always@(*) begin
		state_next=state;
		for(i=0;i<=7;i=i+1)
			bottom_next[i]=bottom[i];
		for(i=0;i<=63;i=i+1)
			A_next[i]=A[i];
		player_next=player;
		pos_next=pos;
		rst_n_next=rst_n;
		wait_count_next=wait_count; 
		 
		case (state)
			init: begin
				state_next=put;
				for(i=0;i<=7;i=i+1)
					bottom_next[i]=3'b111;
				for(i=0;i<=63;i=i+1)
					A_next[i]=2'b00;
			end
			put: begin
				for (i=0;i<=7;i=i+1)
					if(press[i]) begin
						doing = 1; // doing means lock
						if(bottom[i]) begin
							bottom_next[i]=bottom[i]-1;
							A_next[bottom[i]*8+i]=player?2'b01:2'b10;
							player_next=player+1;
							pos_next=bottom[i]*8+i;
							
							state_next=Ring;
							
							
						end
						doing = 0; // unlock
					end
			end
				
			pause: begin
					end
			Ring: begin if (ringdone) state_next=Wait; end
			Wait: begin
				// check if LCD done
				if (done) begin
					wait_count_next=17'b0;
					state_next=player&&mode?AIWAIT:put;
				end else begin
					wait_count_next=wait_count+1;
				end
			end
			AIWAIT: begin
				state_next=AI;
			end
			AI: begin
				doing = 1; // doing means lock
				i=decide;
				if(bottom[i]) begin
					bottom_next[i]=bottom[i]-1;
					A_next[bottom[i]*8+i]=player?2'b01:2'b10;
					player_next=player+1;
					pos_next=bottom[i]*8+i;
							
					state_next=Ring;
							
							
				end
				doing = 0; // unlock
			end
		endcase
	end
	
	assign D16=over;
	
	
	parameter P2 = {4'd11,4'b01}, 
				 P1 = {4'd11,4'b01}, 
				 AI_disp = {4'd15,4'd13};
	wire [15:0]chr;
	wire PLAYER;
	assign PLAYER=over^|player;
	assign chr[15:12]=mode&&PLAYER?4'd15:4'd11;
	assign chr[11:8]=mode&&PLAYER?4'd13:PLAYER+1;
	assign chr[3:0]=over?4'd12:4'd10;
	assign chr[7:4]=4'd10;
	//display_ct dis (.clk(clk3),.bcds({1'b0,bottom[5],1'b0,bottom[4],1'b0,bottom[3],1'b0,bottom[2]}), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds({A[63],A[55],A[47],A[39],A[31],A[23],A[15],A[7]}), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds({lr,ud,lurd,ldru}), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds({lr,ud,1'b0,pos[5:3],1'b0,pos[2:0]}), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds({3'b000,over,2'b0,state==put,player,1'b0,pos[5:3],1'b0,pos[2:0]}), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds({4'd11, 2'b0, over? (player? 2'b01 : 2'b10) : (player? 2'b10 : 2'b01), over? 4'd12 : 4'd10, 4'd10}), .dig(dig), .seg(seg));
	display_ct dis (.clk(clk3),.bcds(chr), .dig(dig), .seg(seg));
	//display_ct dis (.clk(clk3),.bcds(score[4]>=sss), .dig(dig), .seg(seg));
	/*PAC_MAN pcman(clk, 1, LCD_DATA, LCD_ENABLE,
       LCD_RW, LCD_RSTN, LCD_CS1, LCD_CS2, LCD_DI,6'b0,|sp);
	 */
	/*draw d(.LCD_CLK(clk), .RESETN(1), .LCD_DATA(LCD_DATA), .LCD_ENABLE(LCD_ENABLE),
       .LCD_RW(LCD_RW), .LCD_RSTN(LCD_RSTN), .LCD_CS1(LCD_CS1), .LCD_CS2(LCD_CS2), .LCD_DI(LCD_DI),.data(data));*/
	draw2 d(.LCD_CLK(clk), .RESETN(rst_n2), .LCD_DATA(LCD_DATA), .LCD_ENABLE(LCD_ENABLE),
       .LCD_RW(LCD_RW), .LCD_RSTN(LCD_RSTN), .LCD_CS1(LCD_CS1), .LCD_CS2(LCD_CS2), .LCD_DI(LCD_DI),.pos(pos),.image(A[pos]), .done(done));

	note_generator ng (.clk(LCD_CLK), .rst_n(reset_n), .note_div(note_div), .audio_left(audio_left), .audio_right(audio_right));
	buzzer_control bc (.clk(LCD_CLK),.rst_n(reset_n),.audio_in_left(audio_left),.audio_in_right(audio_right), 
								  .audio_appsel(audio_appsel), .audio_sysclk(audio_sysclk), .audio_bck(audio_bck),
								  .audio_ws(audio_ws), .audio_data(audio_data));


endmodule
   
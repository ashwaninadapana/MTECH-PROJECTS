module mux_128x1(I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31, I32, I33, I34, I35, I36, I37, I38, I39, I40, I41, I42, I43, I44, I45, I46, I47, I48, I49, I50, I51, I52, I53, I54, I55, I56, I57, I58, I59, I60, I61, I62, I63, I64, I65, I66, I67, I68, I69, I70, I71, I72, I73, I74, I75, I76, I77, I78, I79, I80, I81, I82, I83, I84, I85, I86, I87, I88, I89, I90, I91, I92, I93, I94, I95, I96, I97, I98, I99, I100, I101, I102, I103, I104, I105, I106, I107, I108, I109, I110, I111, I112, I113, I114, I115, I116, I117, I118, I119, I120, I121, I122, I123, I124, I125, I126, I127 , select, y);
	input[199:0] I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31, I32, I33, I34, I35, I36, I37, I38, I39, I40, I41, I42, I43, I44, I45, I46, I47, I48, I49, I50, I51, I52, I53, I54, I55, I56, I57, I58, I59, I60, I61, I62, I63, I64, I65, I66, I67, I68, I69, I70, I71, I72, I73, I74, I75, I76, I77, I78, I79, I80, I81, I82, I83, I84, I85, I86, I87, I88, I89, I90, I91, I92, I93, I94, I95, I96, I97, I98, I99, I100, I101, I102, I103, I104, I105, I106, I107, I108, I109, I110, I111, I112, I113, I114, I115, I116, I117, I118, I119, I120, I121, I122, I123, I124, I125, I126, I127 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1 or I2 or I3 or I4 or I5 or I6 or I7 or I8 or I9 or I10 or I11 or I12 or I13 or I14 or I15 or I16 or I17 or I18 or I19 or I20 or I21 or I22 or I23 or I24 or I25 or I26 or I27 or I28 or I29 or I30 or I31 or I32 or I33 or I34 or I35 or I36 or I37 or I38 or I39 or I40 or I41 or I42 or I43 or I44 or I45 or I46 or I47 or I48 or I49 or I50 or I51 or I52 or I53 or I54 or I55 or I56 or I57 or I58 or I59 or I60 or I61 or I62 or I63 or I64 or I65 or I66 or I67 or I68 or I69 or I70 or I71 or I72 or I73 or I74 or I75 or I76 or I77 or I78 or I79 or I80 or I81 or I82 or I83 or I84 or I85 or I86 or I87 or I88 or I89 or I90 or I91 or I92 or I93 or I94 or I95 or I96 or I97 or I98 or I99 or I100 or I101 or I102 or I103 or I104 or I105 or I106 or I107 or I108 or I109 or I110 or I111 or I112 or I113 or I114 or I115 or I116 or I117 or I118 or I119 or I120 or I121 or I122 or I123 or I124 or I125 or I126 or I127  or select)
		begin
			case(select)
				7'b0000000:y = I0;
				7'b0000001:y = I1;
				7'b0000010:y = I2;
				7'b0000011:y = I3;
				7'b0000100:y = I4;
				7'b0000101:y = I5;
				7'b0000110:y = I6;
				7'b0000111:y = I7;
				7'b0001000:y = I8;
				7'b0001001:y = I9;
				7'b0001010:y = I10;
				7'b0001011:y = I11;
				7'b0001100:y = I12;
				7'b0001101:y = I13;
				7'b0001110:y = I14;
				7'b0001111:y = I15;
				7'b0010000:y = I16;
				7'b0010001:y = I17;
				7'b0010010:y = I18;
				7'b0010011:y = I19;
				7'b0010100:y = I20;
				7'b0010101:y = I21;
				7'b0010110:y = I22;
				7'b0010111:y = I23;
				7'b0011000:y = I24;
				7'b0011001:y = I25;
				7'b0011010:y = I26;
				7'b0011011:y = I27;
				7'b0011100:y = I28;
				7'b0011101:y = I29;
				7'b0011110:y = I30;
				7'b0011111:y = I31;
				7'b0100000:y = I32;
				7'b0100001:y = I33;
				7'b0100010:y = I34;
				7'b0100011:y = I35;
				7'b0100100:y = I36;
				7'b0100101:y = I37;
				7'b0100110:y = I38;
				7'b0100111:y = I39;
				7'b0101000:y = I40;
				7'b0101001:y = I41;
				7'b0101010:y = I42;
				7'b0101011:y = I43;
				7'b0101100:y = I44;
				7'b0101101:y = I45;
				7'b0101110:y = I46;
				7'b0101111:y = I47;
				7'b0110000:y = I48;
				7'b0110001:y = I49;
				7'b0110010:y = I50;
				7'b0110011:y = I51;
				7'b0110100:y = I52;
				7'b0110101:y = I53;
				7'b0110110:y = I54;
				7'b0110111:y = I55;
				7'b0111000:y = I56;
				7'b0111001:y = I57;
				7'b0111010:y = I58;
				7'b0111011:y = I59;
				7'b0111100:y = I60;
				7'b0111101:y = I61;
				7'b0111110:y = I62;
				7'b0111111:y = I63;
				7'b1000000:y = I64;
				7'b1000001:y = I65;
				7'b1000010:y = I66;
				7'b1000011:y = I67;
				7'b1000100:y = I68;
				7'b1000101:y = I69;
				7'b1000110:y = I70;
				7'b1000111:y = I71;
				7'b1001000:y = I72;
				7'b1001001:y = I73;
				7'b1001010:y = I74;
				7'b1001011:y = I75;
				7'b1001100:y = I76;
				7'b1001101:y = I77;
				7'b1001110:y = I78;
				7'b1001111:y = I79;
				7'b1010000:y = I80;
				7'b1010001:y = I81;
				7'b1010010:y = I82;
				7'b1010011:y = I83;
				7'b1010100:y = I84;
				7'b1010101:y = I85;
				7'b1010110:y = I86;
				7'b1010111:y = I87;
				7'b1011000:y = I88;
				7'b1011001:y = I89;
				7'b1011010:y = I90;
				7'b1011011:y = I91;
				7'b1011100:y = I92;
				7'b1011101:y = I93;
				7'b1011110:y = I94;
				7'b1011111:y = I95;
				7'b1100000:y = I96;
				7'b1100001:y = I97;
				7'b1100010:y = I98;
				7'b1100011:y = I99;
				7'b1100100:y = I100;
				7'b1100101:y = I101;
				7'b1100110:y = I102;
				7'b1100111:y = I103;
				7'b1101000:y = I104;
				7'b1101001:y = I105;
				7'b1101010:y = I106;
				7'b1101011:y = I107;
				7'b1101100:y = I108;
				7'b1101101:y = I109;
				7'b1101110:y = I110;
				7'b1101111:y = I111;
				7'b1110000:y = I112;
				7'b1110001:y = I113;
				7'b1110010:y = I114;
				7'b1110011:y = I115;
				7'b1110100:y = I116;
				7'b1110101:y = I117;
				7'b1110110:y = I118;
				7'b1110111:y = I119;
				7'b1111000:y = I120;
				7'b1111001:y = I121;
				7'b1111010:y = I122;
				7'b1111011:y = I123;
				7'b1111100:y = I124;
				7'b1111101:y = I125;
				7'b1111110:y = I126;
				7'b1111111:y = I127;
			endcase
		end
endmodule

module mux_16x1(I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 , select, y);
	input[199:0] I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1 or I2 or I3 or I4 or I5 or I6 or I7 or I8 or I9 or I10 or I11 or I12 or I13 or I14 or I15  or select)
		begin
			case(select)
				4'b0000:y = I0;
				4'b0001:y = I1;
				4'b0010:y = I2;
				4'b0011:y = I3;
				4'b0100:y = I4;
				4'b0101:y = I5;
				4'b0110:y = I6;
				4'b0111:y = I7;
				4'b1000:y = I8;
				4'b1001:y = I9;
				4'b1010:y = I10;
				4'b1011:y = I11;
				4'b1100:y = I12;
				4'b1101:y = I13;
				4'b1110:y = I14;
				4'b1111:y = I15;
			endcase
		end
endmodule

module mux_32x1(I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31 , select, y);
	input[199:0] I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1 or I2 or I3 or I4 or I5 or I6 or I7 or I8 or I9 or I10 or I11 or I12 or I13 or I14 or I15 or I16 or I17 or I18 or I19 or I20 or I21 or I22 or I23 or I24 or I25 or I26 or I27 or I28 or I29 or I30 or I31  or select)
		begin
			case(select)
				5'b00000:y = I0;
				5'b00001:y = I1;
				5'b00010:y = I2;
				5'b00011:y = I3;
				5'b00100:y = I4;
				5'b00101:y = I5;
				5'b00110:y = I6;
				5'b00111:y = I7;
				5'b01000:y = I8;
				5'b01001:y = I9;
				5'b01010:y = I10;
				5'b01011:y = I11;
				5'b01100:y = I12;
				5'b01101:y = I13;
				5'b01110:y = I14;
				5'b01111:y = I15;
				5'b10000:y = I16;
				5'b10001:y = I17;
				5'b10010:y = I18;
				5'b10011:y = I19;
				5'b10100:y = I20;
				5'b10101:y = I21;
				5'b10110:y = I22;
				5'b10111:y = I23;
				5'b11000:y = I24;
				5'b11001:y = I25;
				5'b11010:y = I26;
				5'b11011:y = I27;
				5'b11100:y = I28;
				5'b11101:y = I29;
				5'b11110:y = I30;
				5'b11111:y = I31;
			endcase
		end
endmodule

module mux_64x1(I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31, I32, I33, I34, I35, I36, I37, I38, I39, I40, I41, I42, I43, I44, I45, I46, I47, I48, I49, I50, I51, I52, I53, I54, I55, I56, I57, I58, I59, I60, I61, I62, I63 , select, y);
	input[199:0] I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31, I32, I33, I34, I35, I36, I37, I38, I39, I40, I41, I42, I43, I44, I45, I46, I47, I48, I49, I50, I51, I52, I53, I54, I55, I56, I57, I58, I59, I60, I61, I62, I63 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1 or I2 or I3 or I4 or I5 or I6 or I7 or I8 or I9 or I10 or I11 or I12 or I13 or I14 or I15 or I16 or I17 or I18 or I19 or I20 or I21 or I22 or I23 or I24 or I25 or I26 or I27 or I28 or I29 or I30 or I31 or I32 or I33 or I34 or I35 or I36 or I37 or I38 or I39 or I40 or I41 or I42 or I43 or I44 or I45 or I46 or I47 or I48 or I49 or I50 or I51 or I52 or I53 or I54 or I55 or I56 or I57 or I58 or I59 or I60 or I61 or I62 or I63  or select)
		begin
			case(select)
				6'b000000:y = I0;
				6'b000001:y = I1;
				6'b000010:y = I2;
				6'b000011:y = I3;
				6'b000100:y = I4;
				6'b000101:y = I5;
				6'b000110:y = I6;
				6'b000111:y = I7;
				6'b001000:y = I8;
				6'b001001:y = I9;
				6'b001010:y = I10;
				6'b001011:y = I11;
				6'b001100:y = I12;
				6'b001101:y = I13;
				6'b001110:y = I14;
				6'b001111:y = I15;
				6'b010000:y = I16;
				6'b010001:y = I17;
				6'b010010:y = I18;
				6'b010011:y = I19;
				6'b010100:y = I20;
				6'b010101:y = I21;
				6'b010110:y = I22;
				6'b010111:y = I23;
				6'b011000:y = I24;
				6'b011001:y = I25;
				6'b011010:y = I26;
				6'b011011:y = I27;
				6'b011100:y = I28;
				6'b011101:y = I29;
				6'b011110:y = I30;
				6'b011111:y = I31;
				6'b100000:y = I32;
				6'b100001:y = I33;
				6'b100010:y = I34;
				6'b100011:y = I35;
				6'b100100:y = I36;
				6'b100101:y = I37;
				6'b100110:y = I38;
				6'b100111:y = I39;
				6'b101000:y = I40;
				6'b101001:y = I41;
				6'b101010:y = I42;
				6'b101011:y = I43;
				6'b101100:y = I44;
				6'b101101:y = I45;
				6'b101110:y = I46;
				6'b101111:y = I47;
				6'b110000:y = I48;
				6'b110001:y = I49;
				6'b110010:y = I50;
				6'b110011:y = I51;
				6'b110100:y = I52;
				6'b110101:y = I53;
				6'b110110:y = I54;
				6'b110111:y = I55;
				6'b111000:y = I56;
				6'b111001:y = I57;
				6'b111010:y = I58;
				6'b111011:y = I59;
				6'b111100:y = I60;
				6'b111101:y = I61;
				6'b111110:y = I62;
				6'b111111:y = I63;
			endcase
		end
endmodule

module mux_4x1(I0, I1, I2, I3 , select, y);
	input[199:0] I0, I1, I2, I3 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1 or I2 or I3  or select)
		begin
			case(select)
				2'b00:y = I0;
				2'b01:y = I1;
				2'b10:y = I2;
				2'b11:y = I3;
			endcase
		end
endmodule

module mux_2x1(I0, I1 , select, y);
	input[199:0] I0, I1 ;
	input select;
	output reg[199:0] y;
	always @ (I0 or I1  or select)
		begin
			case(select)
				1'b0:y = I0;
				1'b1:y = I1;
			endcase
		end
endmodule

module ADDER1( out, A, B , select );
	input[199:0] A;
	input[199:0] B;
	output reg[199:0] out;
	input select ;
	always @(select or A or B)
	begin
		case(select)
			1'b0 : out = A + B;
			1'b1 : out = A - B;
	endcase
	end
endmodule

module MULT1( out, A, B);
	input[199:0] A;
	input[199:0] B;
	output reg[199:0] out;
	always @(A or B)
	begin
		out = A * B;
	end
endmodule

module MULT2( out, A, B);
	input[199:0] A;
	input[199:0] B;
	output reg[199:0] out;
	always @(A or B)
	begin
		out = A * B;
	end
endmodule

module COMP1( out, A, B , select );
	input[199:0] A;
	input[199:0] B;
	output reg[199:0] out;
	input select ;
	always @(select or A or B)
	begin
		case(select)
			2'b00 : out = A < B;
			2'b01 : out = A > B;
			2'b10 : out = (A == B);
	endcase
	end
endmodule

module SHIFT( out, A, B , select );
	input[199:0] A;
	input[199:0] B;
	output reg[199:0] out;
	input select ;
	always @(select or A or B)
	begin
		case(select)
			1'b0 : out = A >> B;
			1'b1 : out = A << B;
	endcase
	end
endmodule

module fsm( clk, rst , OP1, RP1, RP2, RP3);
	input clk, rst;

	output reg[80:0] OP1;
	input[15:0] RP1;
	input[15:0] RP2;
	input[15:0] RP3;


	reg[199:0] R0;
	reg[199:0] R1;
	reg[199:0] R2;
	reg[199:0] R3;
	reg[199:0] R4;
	reg[199:0] R5;
	reg[199:0] R6;
	reg[199:0] R7;
	reg[199:0] R8;
	reg[199:0] R9;
	reg[199:0] R10;
	reg[199:0] R11;
	reg[199:0] R12;
	reg[199:0] R13;
	reg[199:0] R14;
	reg[199:0] R15;
	reg[199:0] R16;
	reg[199:0] R17;
	reg[199:0] R18;
	reg[199:0] R19;
	reg[199:0] R20;
	reg[199:0] R21;
	reg[199:0] R22;
	reg[199:0] R23;
	reg[199:0] R24;
	reg[199:0] R25;
	reg[199:0] R26;
	reg[199:0] R27;
	reg[199:0] R28;
	reg[199:0] R29;
	reg[199:0] R30;
	reg[199:0] R31;
	reg[199:0] R32;
	reg[199:0] R33;
	reg[199:0] R34;
	reg[199:0] R35;
	reg[199:0] R36;
	reg[199:0] R37;
	reg[199:0] R38;
	reg[199:0] R39;
	reg[199:0] R40;
	reg[199:0] R41;
	reg[199:0] R42;
	reg[199:0] R43;
	reg[199:0] R44;
	reg[199:0] R45;
	reg[199:0] R46;
	reg[199:0] R47;
	reg[199:0] R48;
	reg[199:0] R49;
	reg[199:0] R50;
	reg[199:0] R51;
	reg[199:0] R52;
	reg[199:0] R53;
	reg[199:0] R54;
	reg[199:0] R55;
	reg[199:0] R56;
	reg[199:0] R57;
	reg[199:0] R58;
	reg[199:0] R59;
	reg[199:0] R60;
	reg[199:0] R61;
	reg[199:0] R62;
	reg[199:0] R63;
	reg[199:0] R64;
	reg[199:0] R65;
	reg[199:0] R66;
	reg[199:0] R67;
	reg[199:0] R68;
	reg[199:0] R69;
	reg[199:0] R70;
	reg[199:0] R71;
	reg[199:0] R72;
	reg[199:0] R73;
	reg[199:0] R74;
	reg[199:0] R75;
	reg[199:0] R76;
	reg[199:0] R77;
	reg[199:0] R78;
	reg[199:0] R79;
	reg[199:0] R80;
	reg[199:0] R81;
	reg[199:0] R82;
	reg[199:0] R83;
	reg[199:0] R84;
	reg[199:0] R85;
	reg[199:0] R86;
	reg[199:0] R87;
	reg[199:0] R88;
	reg[199:0] R89;
	reg[199:0] R90;
	reg[199:0] R91;
	reg[199:0] R92;
	reg[199:0] R93;
	reg[199:0] R94;
	reg[199:0] R95;
	reg[199:0] R96;
	reg[199:0] R97;
	reg[199:0] R98;
	reg[199:0] R99;
	reg[199:0] R100;
	reg[199:0] R101;
	reg[199:0] R102;
	reg[199:0] R103;
	reg[199:0] R104;
	reg[199:0] R105;
	reg[199:0] R106;
	reg[199:0] R107;
	reg[199:0] R108;
	reg[199:0] R109;
	reg[199:0] R110;
	reg[199:0] R111;
	reg[199:0] R112;
	reg[199:0] R113;
	reg[199:0] R114;
	reg[199:0] R115;
	reg[199:0] R116;
	reg[199:0] R117;
	reg[199:0] R118;
	reg[199:0] R119;
	reg[199:0] R120;
	reg[199:0] R121;
	reg[199:0] R122;
	reg[199:0] R123;
	reg[199:0] R124;
	reg[199:0] R125;
	reg[199:0] R126;
	reg[199:0] R127;
	reg[199:0] R128;
	reg[199:0] R129;
	reg[199:0] R130;
	reg[199:0] R131;
	reg[199:0] R132;
	reg[199:0] R133;
	reg[199:0] R134;
	reg[199:0] R135;
	reg[199:0] R136;
	reg[199:0] R137;
	reg[199:0] R138;
	reg[199:0] R139;
	reg[199:0] R140;
	reg[199:0] R141;
	reg[199:0] R142;
	reg[199:0] R143;
	reg[199:0] R144;
	reg[199:0] R145;
	reg[199:0] R146;
	reg[199:0] R147;
	reg[199:0] R148;
	reg[199:0] R149;
	reg[199:0] R150;
	reg[199:0] R151;
	reg[199:0] R152;
	reg[199:0] R153;
	reg[199:0] R154;
	reg[199:0] R155;
	reg[199:0] R156;
	reg[199:0] R157;
	reg[199:0] R158;
	reg[199:0] R159;
	reg[199:0] R160;
	reg[199:0] R161;
	reg[199:0] R162;
	reg[199:0] R163;
	reg[199:0] R164;
	reg[199:0] R165;
	reg[199:0] R166;
	reg[199:0] R167;
	reg[199:0] R168;
	reg[199:0] R169;
	reg[199:0] R170;
	reg[199:0] R171;
	reg[199:0] R172;
	reg[199:0] R173;
	reg[199:0] R174;
	reg[199:0] R175;
	reg[199:0] R176;
	reg[199:0] R177;
	reg[199:0] R178;
	reg[199:0] R179;
	reg[199:0] R180;
	reg[199:0] R181;
	reg[199:0] R182;
	reg[199:0] R183;
	reg[199:0] R184;
	reg[199:0] R185;
	reg[199:0] R186;
	reg[199:0] R187;
	reg[199:0] R188;
	reg[199:0] R189;
	reg[199:0] R190;
	reg[199:0] R191;
	reg[199:0] R192;
	reg[199:0] R193;
	reg[199:0] R194;
	reg[199:0] R195;
	reg[199:0] R196;
	reg[199:0] R197;
	reg[199:0] R198;
	reg[199:0] R199;
	reg[199:0] R200;


	reg[6:0] select1;
	reg[0:0] select10;
	reg[1:0] select11;
	reg[1:0] select12;
	reg[6:0] select2;
	reg[3:0] select3;
	reg[3:0] select4;
	reg[4:0] select5;
	reg[4:0] select6;
	reg[5:0] select7;
	reg[3:0] select8;
	reg[1:0] select9;
	reg[0:0] fu0_select;
	reg[1:0] fu1_select;
	reg[0:0] fu2_select;

	wire[199:0] M1ADDER1;
	wire[199:0] M2ADDER1;
	wire[199:0] M3MULT1;
	wire[199:0] M4MULT1;
	wire[199:0] M5MULT2;
	wire[199:0] M6MULT2;
	wire[199:0] M7SHIFT;
	wire[199:0] M8SHIFT;
	wire[199:0] R0_wire;
	wire[199:0] R1_wire;
	wire[199:0] R3_wire;
	wire[199:0] R4_wire;

	wire[199:0] ADDER1_wire;
	wire[199:0] MULT1_wire;
	wire[199:0] MULT2_wire;
	wire[199:0] COMP1_wire;
	wire[199:0] SHIFT_wire;

	reg[6:0] state;

	//FU instantiation
	ADDER1 fu0(ADDER1_wire, M1ADDER1, M2ADDER1, fu0_select);
	COMP1 fu1(COMP1_wire, R0, R1, fu1_select);
	MULT1 fu2(MULT1_wire, M3MULT1, M4MULT1);
	MULT2 fu3(MULT2_wire, M5MULT2, M6MULT2);
	SHIFT fu4(SHIFT_wire, M7SHIFT, M8SHIFT, fu2_select);
	//MUX instantiation
	mux_128x1 M1(R0,  R3,  R7,  R12,  R23,  R25,  R28,  R33,  R34,  R36,  R37,  R39,  R41,  R46,  R47,  R49,  R50,  R54,  R59,  R60,  R62,  R63,  R64,  R65,  R67,  R68,  R70,  R72,  R77,  R79,  R84,  R85,  R87,  R88,  R89,  R91,  R93,  R95,  R97,  R103,  R105,  R110,  R111,  R113,  R114,  R115,  R116,  R118,  R119,  R121,  R123,  R129,  R131,  R136,  R137,  R139,  R140,  R141,  R143,  R144,  R147,  R150,  R152,  R157,  R158,  R160,  R161,  R165,  R171,  R172,  R174,  R175,  R176,  R177,  R179,  R180,  R181,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select1, M1ADDER1);
	mux_2x1 M10(ADDER1_wire,  SHIFT_wire,  select10, R1_wire);
	mux_4x1 M11(ADDER1_wire,  MULT2_wire,  SHIFT_wire,  1'bz,  select11, R3_wire);
	mux_4x1 M12(ADDER1_wire,  MULT2_wire,  SHIFT_wire,  1'bz,  select12, R4_wire);
	mux_128x1 M2(R1,  R2,  R3,  R4,  R8,  R9,  R10,  R13,  R14,  R15,  R17,  R19,  R21,  R24,  R26,  R29,  R30,  R31,  R35,  R38,  R40,  R42,  R43,  R44,  R48,  R51,  R55,  R56,  R57,  R61,  R66,  R71,  R73,  R74,  R75,  R78,  R80,  R81,  R82,  R86,  R90,  R92,  R94,  R96,  R98,  R99,  R100,  R104,  R106,  R107,  R108,  R112,  R117,  R120,  R122,  R124,  R125,  R126,  R130,  R132,  R133,  R134,  R138,  R142,  R145,  R148,  R149,  R151,  R153,  R154,  R155,  R159,  R162,  R166,  R167,  R168,  R169,  R173,  R178,  R182,  R183,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select2, M2ADDER1);
	mux_16x1 M3(R1,  R3,  R4,  R26,  R77,  R78,  R122,  R130,  R151,  R165,  R166,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select3, M3MULT1);
	mux_16x1 M4(R0,  R2,  R8,  R13,  R29,  R55,  R58,  R79,  R80,  R98,  R124,  R132,  R153,  R167,  1'bz,  1'bz,  select4, M4MULT1);
	mux_32x1 M5(R3,  R25,  R39,  R40,  R70,  R71,  R83,  R97,  R103,  R104,  R109,  R121,  R129,  R135,  R147,  R150,  R156,  R170,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select5, M5MULT2);
	mux_32x1 M6(R0,  R4,  R7,  R12,  R16,  R28,  R32,  R41,  R42,  R45,  R54,  R72,  R73,  R105,  R106,  R123,  R131,  R152,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select6, M6MULT2);
	mux_64x1 M7(R0,  R1,  R3,  R4,  R10,  R15,  R21,  R23,  R24,  R31,  R44,  R52,  R69,  R75,  R76,  R82,  R93,  R94,  R95,  R96,  R100,  R101,  R102,  R108,  R126,  R127,  R128,  R134,  R145,  R148,  R155,  R163,  R164,  R169,  R184,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  1'bz,  select7, M7SHIFT);
	mux_16x1 M8(R1,  R6,  R11,  R18,  R20,  R22,  R27,  R51,  R53,  R57,  R146,  R162,  1'bz,  1'bz,  1'bz,  1'bz,  select8, M8SHIFT);
	mux_4x1 M9(ADDER1_wire,  COMP1_wire,  SHIFT_wire,  1'bz,  select9, R0_wire);

	always @(posedge clk or posedge rst )
		begin
			if( rst )
				begin
						state <= 7'b0000000;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= RP1;
						R1 <= RP2;
						R2 <= RP3;
				end
			else
			begin
				case(state)
					7'b0000000: begin//B11
						state <= 7'b0000001;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= RP1;
						R1 <= RP2;
						R2 <= RP3;
						end
					7'b0000001: begin//B12
						state <= 7'b1101010;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b01;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= RP1;
						R1 <= RP2;
						end
					7'b0000010: begin//B21
						state <= 7'b0010000;
						select1 <= 7'b0000010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b10;
						select2 <= 7'b0000100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000011;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R4 <= R4_wire;
						R1 <= R7;
						R8 <= SHIFT_wire;
						R69 <= MULT1_wire;
						R3 <= R3_wire;
						end
					7'b0000011: begin//B210
						state <= 7'b0000111;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00101;
						select6 <= 5'b01100;
						select7 <= 6'b001011;
						select8 <= 4'b1000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R52 <= R54;
						R73 <= SHIFT_wire;
						end
					7'b0000100: begin//B2100
						state <= 7'b0000101;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						end
					7'b0000101: begin//B2101
						state <= 7'b0000110;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						end
					7'b0000110: begin//B2102
						state <= 7'b1101010;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b01;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R2 <= ADDER1_wire;
						end
					7'b0000111: begin//B211
						state <= 7'b0001000;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000111;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R55 <= SHIFT_wire;
						R75 <= MULT2_wire;
						R0 <= R95;
						end
					7'b0001000: begin//B212
						state <= 7'b0001001;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b001000;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R29 <= SHIFT_wire;
						R95 <= R97;
						end
					7'b0001001: begin//B213
						state <= 7'b0001010;
						select1 <= 7'b0011010;
						select10 <= 1'b0;
						select11 <= 2'b10;
						select12 <= 2'b00;
						select2 <= 7'b0011111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000010;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3_wire;
						R42 <= SHIFT_wire;
						end
					7'b0001010: begin//B214
						state <= 7'b0001011;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000011;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R4 <= R4_wire;
						end
					7'b0001011: begin//B215
						state <= 7'b0001100;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b001101;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3_wire;
						R4 <= R77;
						R78 <= SHIFT_wire;
						end
					7'b0001100: begin//B216
						state <= 7'b0001101;
						select1 <= 7'b0011011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000010;
						select8 <= 4'b0101;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R91 <= SHIFT_wire;
						end
					7'b0001101: begin//B217
						state <= 7'b0001110;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000010;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3;
						R21 <= SHIFT_wire;
						R76 <= ADDER1_wire;
						end
					7'b0001110: begin//B218
						state <= 7'b0001111;
						select1 <= 7'b0010001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b10;
						select2 <= 7'b0011010;
						select3 <= 4'b0011;
						select4 <= 4'b0100;
						select5 <= 5'b00001;
						select6 <= 5'b00101;
						select7 <= 6'b000010;
						select8 <= 4'b1000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R25;
						R26 <= SHIFT_wire;
						R3 <= R3_wire;
						end
					7'b0001111: begin//B219
						state <= 7'b0010001;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000011;
						select3 <= 4'b0010;
						select4 <= 4'b0101;
						select5 <= 5'b00000;
						select6 <= 5'b01010;
						select7 <= 6'b000110;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R30 <= MULT2_wire;
						R31 <= MULT1_wire;
						R3 <= R3;
						R4 <= R4_wire;
						R58 <= ADDER1_wire;
						end
					7'b0010000: begin//B22
						state <= 7'b0011011;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000011;
						select3 <= 4'b0010;
						select4 <= 4'b0010;
						select5 <= 5'b00000;
						select6 <= 5'b00010;
						select7 <= 6'b100010;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R4 <= R3;
						R4 <= R4_wire;
						R184 <= ADDER1_wire;
						end
					7'b0010001: begin//B220
						state <= 7'b0010010;
						select1 <= 7'b0000101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001110;
						select3 <= 4'b0001;
						select4 <= 4'b0110;
						select5 <= 5'b00010;
						select6 <= 5'b00111;
						select7 <= 6'b001110;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R21 <= R39;
						R40 <= SHIFT_wire;
						R56 <= MULT2_wire;
						R57 <= MULT1_wire;
						R3 <= R3_wire;
						end
					7'b0010010: begin//B221
						state <= 7'b0010011;
						select1 <= 7'b0000110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001111;
						select3 <= 4'b0100;
						select4 <= 4'b0111;
						select5 <= 5'b00011;
						select6 <= 5'b01000;
						select7 <= 6'b000010;
						select8 <= 4'b1001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R32 <= ADDER1_wire;
						R43 <= MULT2_wire;
						R59 <= MULT1_wire;
						R76 <= R79;
						R80 <= SHIFT_wire;
						end
					7'b0010011: begin//B222
						state <= 7'b0010100;
						select1 <= 7'b0001011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010100;
						select3 <= 4'b0101;
						select4 <= 4'b1000;
						select5 <= 5'b00000;
						select6 <= 5'b00110;
						select7 <= 6'b001001;
						select8 <= 4'b0101;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3_wire;
						R44 <= MULT2_wire;
						R62 <= SHIFT_wire;
						R81 <= MULT1_wire;
						end
					7'b0010100: begin//B223
						state <= 7'b0010101;
						select1 <= 7'b0001100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b001010;
						select8 <= 4'b0101;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R33 <= MULT2_wire;
						R36 <= SHIFT_wire;
						R45 <= ADDER1_wire;
						R82 <= MULT1_wire;
						end
					7'b0010101: begin//B224
						state <= 7'b0010110;
						select1 <= 7'b0011100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b01001;
						select7 <= 6'b001111;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3_wire;
						R49 <= SHIFT_wire;
						end
					7'b0010110: begin//B225
						state <= 7'b0010111;
						select1 <= 7'b0011101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R46 <= MULT2_wire;
						R83 <= ADDER1_wire;
						R87 <= SHIFT_wire;
						end
					7'b0010111: begin//B226
						state <= 7'b0011000;
						select1 <= 7'b0000111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00110;
						select6 <= 5'b00000;
						select7 <= 6'b010010;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						R96 <= SHIFT_wire;
						end
					7'b0011000: begin//B227
						state <= 7'b0011001;
						select1 <= 7'b0010010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b010011;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R34 <= ADDER1_wire;
						R84 <= MULT2_wire;
						R98 <= SHIFT_wire;
						R96 <= R123;
						end
					7'b0011001: begin//B228
						state <= 7'b0011010;
						select1 <= 7'b0001000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R60 <= ADDER1_wire;
						R124 <= SHIFT_wire;
						end
					7'b0011010: begin//B229
						state <= 7'b0011100;
						select1 <= 7'b0010011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b0011011: begin//B23
						state <= 7'b0100110;
						select1 <= 7'b0000011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b10;
						select2 <= 7'b0000111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000010;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R9 <= MULT2_wire;
						R10 <= MULT1_wire;
						R3 <= R3_wire;
						R184 <= R12;
						R13 <= SHIFT_wire;
						end
					7'b0011100: begin//B230
						state <= 7'b0011101;
						select1 <= 7'b0001101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b1000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R35 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b0011101: begin//B231
						state <= 7'b0011110;
						select1 <= 7'b0011110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R47 <= ADDER1_wire;
						R61 <= SHIFT_wire;
						end
					7'b0011110: begin//B232
						state <= 7'b0011111;
						select1 <= 7'b0001110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R85 <= ADDER1_wire;
						end
					7'b0011111: begin//B233
						state <= 7'b0100000;
						select1 <= 7'b0011111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0110;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b0100000: begin//B234
						state <= 7'b0100001;
						select1 <= 7'b0010100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R48 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b0100001: begin//B235
						state <= 7'b0100010;
						select1 <= 7'b0001001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R63 <= ADDER1_wire;
						R86 <= SHIFT_wire;
						end
					7'b0100010: begin//B236
						state <= 7'b0100011;
						select1 <= 7'b0010101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R37 <= ADDER1_wire;
						end
					7'b0100011: begin//B237
						state <= 7'b0100100;
						select1 <= 7'b0001010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R64 <= ADDER1_wire;
						end
					7'b0100100: begin//B238
						state <= 7'b0100101;
						select1 <= 7'b0001111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R38 <= ADDER1_wire;
						end
					7'b0100101: begin//B239
						state <= 7'b0100111;
						select1 <= 7'b0100000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R50 <= ADDER1_wire;
						end
					7'b0100110: begin//B24
						state <= 7'b0110001;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000011;
						select3 <= 4'b0010;
						select4 <= 4'b0011;
						select5 <= 5'b00000;
						select6 <= 5'b00011;
						select7 <= 6'b000100;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3;
						R4 <= R4_wire;
						R16 <= ADDER1_wire;
						end
					7'b0100111: begin//B240
						state <= 7'b0101000;
						select1 <= 7'b0010110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R88 <= ADDER1_wire;
						end
					7'b0101000: begin//B241
						state <= 7'b0101001;
						select1 <= 7'b0010000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R65 <= ADDER1_wire;
						end
					7'b0101001: begin//B242
						state <= 7'b0101010;
						select1 <= 7'b0100001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0111;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R51 <= ADDER1_wire;
						end
					7'b0101010: begin//B243
						state <= 7'b0101011;
						select1 <= 7'b0010111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R67 <= SHIFT_wire;
						R89 <= ADDER1_wire;
						end
					7'b0101011: begin//B244
						state <= 7'b0101100;
						select1 <= 7'b0100010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0101;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b0101100: begin//B245
						state <= 7'b0101101;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R66 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b0101101: begin//B246
						state <= 7'b0101110;
						select1 <= 7'b0011000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0011110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b0101110: begin//B247
						state <= 7'b0101111;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0010011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						R90 <= SHIFT_wire;
						end
					7'b0101111: begin//B248
						state <= 7'b0110000;
						select1 <= 7'b0100011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R68 <= ADDER1_wire;
						end
					7'b0110000: begin//B249
						state <= 7'b0110010;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0100001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						end
					7'b0110001: begin//B25
						state <= 7'b0111100;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b01;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00100;
						select7 <= 6'b000101;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R14 <= MULT2_wire;
						R15 <= MULT1_wire;
						R3 <= R3_wire;
						R19 <= SHIFT_wire;
						R0 <= R23;
						end
					7'b0110010: begin//B250
						state <= 7'b0110011;
						select1 <= 7'b0011001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R92 <= ADDER1_wire;
						end
					7'b0110011: begin//B251
						state <= 7'b0110100;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						end
					7'b0110100: begin//B252
						state <= 7'b0110101;
						select1 <= 7'b0100101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000001;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R1 <= R1_wire;
						end
					7'b0110101: begin//B253
						state <= 7'b0110110;
						select1 <= 7'b0100100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b1010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R1 <= R93;
						R94 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b0110110: begin//B254
						state <= 7'b0110111;
						select1 <= 7'b0111100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b1010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						R0 <= R147;
						R148 <= SHIFT_wire;
						end
					7'b0110111: begin//B255
						state <= 7'b0111000;
						select1 <= 7'b0000000;
						select10 <= 1'b1;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b01110;
						select6 <= 5'b00000;
						select7 <= 6'b010000;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0;
						R145 <= SHIFT_wire;
						R164 <= ADDER1_wire;
						end
					7'b0111000: begin//B256
						state <= 7'b0111001;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b1001;
						select5 <= 5'b00111;
						select6 <= 5'b00000;
						select7 <= 6'b010001;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R93 <= R0;
						R1 <= R1_wire;
						R149 <= MULT2_wire;
						R163 <= ADDER1_wire;
						end
					7'b0111001: begin//B257
						state <= 7'b0111010;
						select1 <= 7'b0100110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101100;
						select3 <= 4'b0110;
						select4 <= 4'b1010;
						select5 <= 5'b01011;
						select6 <= 5'b01111;
						select7 <= 6'b011111;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R99 <= MULT2_wire;
						R100 <= MULT1_wire;
						R101 <= ADDER1_wire;
						R94 <= R121;
						R122 <= SHIFT_wire;
						end
					7'b0111010: begin//B258
						state <= 7'b0111011;
						select1 <= 7'b0110001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b100000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R102 <= ADDER1_wire;
						R101 <= R103;
						R125 <= MULT2_wire;
						R126 <= MULT1_wire;
						R166 <= SHIFT_wire;
						end
					7'b0111011: begin//B259
						state <= 7'b0111101;
						select1 <= 7'b0110010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110111;
						select3 <= 4'b1010;
						select4 <= 4'b1101;
						select5 <= 5'b01000;
						select6 <= 5'b01101;
						select7 <= 6'b010101;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R102 <= R105;
						R127 <= ADDER1_wire;
						R167 <= SHIFT_wire;
						end
					7'b0111100: begin//B26
						state <= 7'b1000111;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0101;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R17 <= SHIFT_wire;
						R69 <= R70;
						end
					7'b0111101: begin//B260
						state <= 7'b0111110;
						select1 <= 7'b1000011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b010110;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R104 <= SHIFT_wire;
						R107 <= MULT2_wire;
						R128 <= ADDER1_wire;
						R163 <= R165;
						R169 <= MULT1_wire;
						end
					7'b0111110: begin//B261
						state <= 7'b0111111;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001010;
						select3 <= 4'b1001;
						select4 <= 4'b0000;
						select5 <= 5'b01001;
						select6 <= 5'b01110;
						select7 <= 6'b011001;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R106 <= SHIFT_wire;
						R164 <= R0;
						R170 <= ADDER1_wire;
						end
					7'b0111111: begin//B262
						state <= 7'b1000000;
						select1 <= 7'b0100111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b10001;
						select6 <= 5'b00000;
						select7 <= 6'b011010;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R108 <= MULT2_wire;
						R127 <= R129;
						R130 <= SHIFT_wire;
						R168 <= MULT1_wire;
						R0 <= R0_wire;
						end
					7'b1000000: begin//B263
						state <= 7'b1000001;
						select1 <= 7'b0101000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110000;
						select3 <= 4'b0111;
						select4 <= 4'b1011;
						select5 <= 5'b01100;
						select6 <= 5'b10000;
						select7 <= 6'b011100;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R109 <= ADDER1_wire;
						R128 <= R131;
						R132 <= SHIFT_wire;
						R171 <= MULT2_wire;
						end
					7'b1000001: begin//B264
						state <= 7'b1000010;
						select1 <= 7'b0110011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b01010;
						select6 <= 5'b00000;
						select7 <= 6'b011101;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						R133 <= MULT2_wire;
						R134 <= MULT1_wire;
						R145 <= R150;
						R151 <= SHIFT_wire;
						end
					7'b1000010: begin//B265
						state <= 7'b1000011;
						select1 <= 7'b0110100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111011;
						select3 <= 4'b1000;
						select4 <= 4'b1100;
						select5 <= 5'b01111;
						select6 <= 5'b10001;
						select7 <= 6'b010111;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R110 <= MULT2_wire;
						R135 <= ADDER1_wire;
						R148 <= R152;
						R153 <= SHIFT_wire;
						end
					7'b1000011: begin//B266
						state <= 7'b1000100;
						select1 <= 7'b0111101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b01101;
						select6 <= 5'b00000;
						select7 <= 6'b100001;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R113 <= SHIFT_wire;
						R0 <= R0_wire;
						R154 <= MULT2_wire;
						R155 <= MULT1_wire;
						end
					7'b1000100: begin//B267
						state <= 7'b1000101;
						select1 <= 7'b0111110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b011011;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R136 <= MULT2_wire;
						R156 <= ADDER1_wire;
						R174 <= SHIFT_wire;
						end
					7'b1000101: begin//B268
						state <= 7'b1000110;
						select1 <= 7'b0101001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b10000;
						select6 <= 5'b00000;
						select7 <= 6'b011110;
						select8 <= 4'b0011;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R139 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b1000110: begin//B269
						state <= 7'b1001000;
						select1 <= 7'b1000100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b010100;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R111 <= ADDER1_wire;
						R157 <= MULT2_wire;
						R160 <= SHIFT_wire;
						end
					7'b1000111: begin//B27
						state <= 7'b1010010;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00100;
						select6 <= 5'b01011;
						select7 <= 6'b001100;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R24 <= SHIFT_wire;
						R0 <= R72;
						end
					7'b1001000: begin//B270
						state <= 7'b1001001;
						select1 <= 7'b0101010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b011000;
						select8 <= 4'b0100;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R118 <= SHIFT_wire;
						R172 <= ADDER1_wire;
						end
					7'b1001001: begin//B271
						state <= 7'b1001010;
						select1 <= 7'b1000101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						R143 <= SHIFT_wire;
						end
					7'b1001010: begin//B272
						state <= 7'b1001011;
						select1 <= 7'b0110101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R112 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b1001011: begin//B273
						state <= 7'b1001100;
						select1 <= 7'b0111111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R137 <= ADDER1_wire;
						R173 <= SHIFT_wire;
						end
					7'b1001100: begin//B274
						state <= 7'b1001101;
						select1 <= 7'b0110110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R158 <= ADDER1_wire;
						end
					7'b1001101: begin//B275
						state <= 7'b1001110;
						select1 <= 7'b1000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b1001110: begin//B276
						state <= 7'b1001111;
						select1 <= 7'b0101011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R138 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b1001111: begin//B277
						state <= 7'b1010000;
						select1 <= 7'b1000110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R114 <= ADDER1_wire;
						R159 <= SHIFT_wire;
						end
					7'b1010000: begin//B278
						state <= 7'b1010001;
						select1 <= 7'b0101100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R175 <= ADDER1_wire;
						end
					7'b1010001: begin//B279
						state <= 7'b1010011;
						select1 <= 7'b1000111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001011;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R115 <= ADDER1_wire;
						end
					7'b1010010: begin//B28
						state <= 7'b1011101;
						select1 <= 7'b0000100;
						select10 <= 1'b0;
						select11 <= 2'b10;
						select12 <= 2'b00;
						select2 <= 7'b0001101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000010;
						select8 <= 4'b0010;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R3 <= R3_wire;
						R23 <= R28;
						R71 <= SHIFT_wire;
						R74 <= MULT2_wire;
						end
					7'b1010011: begin//B280
						state <= 7'b1010100;
						select1 <= 7'b0110111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R176 <= ADDER1_wire;
						end
					7'b1010100: begin//B281
						state <= 7'b1010101;
						select1 <= 7'b1000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R140 <= ADDER1_wire;
						end
					7'b1010101: begin//B282
						state <= 7'b1010110;
						select1 <= 7'b0101101;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R161 <= ADDER1_wire;
						end
					7'b1010110: begin//B283
						state <= 7'b1010111;
						select1 <= 7'b1001000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R116 <= ADDER1_wire;
						end
					7'b1010111: begin//B284
						state <= 7'b1011000;
						select1 <= 7'b0111000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R177 <= ADDER1_wire;
						end
					7'b1011000: begin//B285
						state <= 7'b1011001;
						select1 <= 7'b1000010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R141 <= ADDER1_wire;
						end
					7'b1011001: begin//B286
						state <= 7'b1011010;
						select1 <= 7'b0101110;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b1011;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R162 <= ADDER1_wire;
						end
					7'b1011010: begin//B287
						state <= 7'b1011011;
						select1 <= 7'b1001001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						R179 <= SHIFT_wire;
						end
					7'b1011011: begin//B288
						state <= 7'b1011100;
						select1 <= 7'b0111001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b1010;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R117 <= SHIFT_wire;
						R0 <= R0_wire;
						end
					7'b1011100: begin//B289
						state <= 7'b1011110;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111001;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						R178 <= SHIFT_wire;
						end
					7'b1011101: begin//B29
						state <= 7'b0000011;
						select1 <= 7'b0000001;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0001010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R3 <= R3_wire;
						R24 <= R41;
						R52 <= ADDER1_wire;
						end
					7'b1011110: begin//B290
						state <= 7'b1011111;
						select1 <= 7'b1001010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001110;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0001;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b1011111: begin//B291
						state <= 7'b1100000;
						select1 <= 7'b0101111;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110100;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R142 <= SHIFT_wire;
						R180 <= ADDER1_wire;
						end
					7'b1100000: begin//B292
						state <= 7'b1100001;
						select1 <= 7'b1001011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1000010;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R119 <= ADDER1_wire;
						end
					7'b1100001: begin//B293
						state <= 7'b1100010;
						select1 <= 7'b0110000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0101101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R181 <= ADDER1_wire;
						end
					7'b1100010: begin//B294
						state <= 7'b1100011;
						select1 <= 7'b0111010;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R120 <= ADDER1_wire;
						end
					7'b1100011: begin//B295
						state <= 7'b1100100;
						select1 <= 7'b1001100;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0110101;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R144 <= ADDER1_wire;
						end
					7'b1100100: begin//B296
						state <= 7'b1100101;
						select1 <= 7'b0111011;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0111000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R182 <= ADDER1_wire;
						end
					7'b1100101: begin//B297
						state <= 7'b1100110;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1001111;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000001;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b1;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						end
					7'b1100110: begin//B298
						state <= 7'b1100111;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b0000000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0100;
						select9 <= 2'b10;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b1;
						R0 <= R0_wire;
						R183 <= SHIFT_wire;
						end
					7'b1100111: begin//B299
						state <= 7'b0000100;
						select1 <= 7'b0000000;
						select10 <= 1'b0;
						select11 <= 2'b00;
						select12 <= 2'b00;
						select2 <= 7'b1010000;
						select3 <= 4'b0000;
						select4 <= 4'b0000;
						select5 <= 5'b00000;
						select6 <= 5'b00000;
						select7 <= 6'b000000;
						select8 <= 4'b0000;
						select9 <= 2'b00;
						fu0_select <= 1'b0;
						fu1_select <= 2'b00;
						fu2_select <= 1'b0;
						R0 <= R0_wire;
						end
					7'b1101000: begin//B31
						state <= 7'b1101001;
						OP1 <= R0;
						end
					7'b1101001: begin//B32
						state <= 7'b0000000;
						end
					7'b1101010: begin//C11
						R0 <= R0_wire;
						if(R0 == 0)
							begin
								state <= 7'b1101000;
								select1 <= 7'b0000000;
								select10 <= 1'b0;
								select11 <= 2'b00;
								select12 <= 2'b00;
								select2 <= 7'b0000000;
								select3 <= 4'b0000;
								select4 <= 4'b0000;
								select5 <= 5'b00000;
								select6 <= 5'b00000;
								select7 <= 6'b000000;
								select8 <= 4'b0000;
								select9 <= 2'b00;
								fu0_select <= 1'b0;
								fu1_select <= 2'b00;
								fu2_select <= 1'b0;
							end
						else
							begin
								state <= 7'b0000010;
								select1 <= 7'b0000001;
								select10 <= 1'b0;
								select11 <= 2'b00;
								select12 <= 2'b01;
								select2 <= 7'b0000010;
								select3 <= 4'b0010;
								select4 <= 4'b0001;
								select5 <= 5'b00000;
								select6 <= 5'b00001;
								select7 <= 6'b000001;
								select8 <= 4'b0001;
								select9 <= 2'b00;
								fu0_select <= 1'b0;
								fu1_select <= 2'b00;
								fu2_select <= 1'b0;
							end
						end
				endcase
		end
	end
endmodule

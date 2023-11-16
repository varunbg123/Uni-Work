/*
	Converter Module Testbench
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 28/04/2023
	
	Description
	-----------
	This will take in 2 values, data and the number of button presses

*/

`timescale 1ns/100ps

module DataConverter_tb;

//inputs
reg [5:0]data_out;
reg [4:0]buttoncount;
reg dataconvertconfirm;

//outputs
wire [5:0]post_data;

//reg [4:0] index;
reg [7:0] char_array [35:0];

integer i;

DataConverter DUT(
	.data_out(data_out),
	.buttoncount(buttoncount),
	.post_data(post_data),
	.dataconvertconfirm(dataconvertconfirm)
	
);

initial begin
	char_array[0] = "0";
	char_array[1] = "1";
	char_array[2] = "2";
	char_array[3] = "3";
	char_array[4] = "4";
	char_array[5] = "5";
	char_array[6] = "6";
	char_array[7] = "7";
	char_array[8] = "8";
	char_array[9] = "9";
	char_array[10] = "A";
	char_array[11] = "B";
	char_array[12] = "C";
	char_array[13] = "D";
	char_array[14] = "E";
	char_array[15] = "F";
	char_array[16] = "G";
	char_array[17] = "H";
	char_array[18] = "I";
	char_array[19] = "J";
	char_array[20] = "K";
	char_array[21] = "L";
	char_array[22] = "M";
	char_array[23] = "N";
	char_array[24] = "O";
	char_array[25] = "P";
	char_array[26] = "Q";
	char_array[27] = "R";
	char_array[28] = "S";
	char_array[29] = "T";
	char_array[30] = "U";
	char_array[31] = "V";
	char_array[32] = "W";
	char_array[33] = "X";
	char_array[34] = "Y";
	char_array[35] = "Z";

	dataconvertconfirm = 1;
	
	$monitor("Loop: %d, buttoncount: %d, data_out: %d, post_data: %d, Letter: %s\n", i, buttoncount, data_out, post_data, char_array[post_data]);
  
	//initial dont do anything
	buttoncount = 0;
	#10;
	//testing for E and T
	buttoncount = 1;
	for (i = 0; i < 2; i = i + 1)begin	
		data_out = i;
		#10;
	end
	buttoncount = 0;
	#20;
	
	// Testing for A, I, M, N 
	buttoncount = 2;
	for (i = 0; i < 4; i = i + 1)begin
		data_out = i;
		#10;
	end
	buttoncount = 0;
	#20;
	
	// Testing for S, U, R, W, D, K, G, O
	buttoncount = 3;
	for (i = 0; i < 8; i = i + 1)begin
		data_out = i;
		#10;
	end
	buttoncount = 0;
	#20;
	
	// Testing for H, V, F, L, P, J, B, X, C, Y, Z, Q
	buttoncount = 4;
	for (i = 0; i < 32; i = i + 1)begin
		data_out = i;
		#10;
	end
	buttoncount = 0;
	#20;	
	
	// Testing for 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	buttoncount = 5;
	for (i = 0; i < 64; i = i + 1)begin
		data_out = i;
		#10;
	end
	

end

endmodule

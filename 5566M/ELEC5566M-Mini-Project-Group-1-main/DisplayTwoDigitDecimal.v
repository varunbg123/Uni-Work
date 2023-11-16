/*
	2 Digit decimal on 2 Hex Displays
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 27/04/2023
	
	Description
	-----------
	This should take in the a decimal value 
	and displays it on 2 hex displays

*/

module DisplayTwoDigitDecimal#(
	parameter INVERT_OUTPUT = 1
)(
	//get the input data 
	input [5:0] getdata,
	//output to hex display one and two
	output reg [6:0]digit1,
	output reg [6:0]digit2
);

//creating an array for digit_encoding to happen
wire [6:0] DIGIT_ENCODING [0:9];

assign DIGIT_ENCODING[0] = 7'b0111111;  //0
assign DIGIT_ENCODING[1] = 7'b0000110;  //1
assign DIGIT_ENCODING[2] = 7'b1011011;  //2
assign DIGIT_ENCODING[3] = 7'b1001111;  //3
assign DIGIT_ENCODING[4] = 7'b1100110;  //4
assign DIGIT_ENCODING[5] = 7'b1101101;  //5
assign DIGIT_ENCODING[6] = 7'b1111101;  //6
assign DIGIT_ENCODING[7] = 7'b0000111;  //7
assign DIGIT_ENCODING[8] = 7'b1111111;  //8
assign DIGIT_ENCODING[9] = 7'b1100111;  //9

//Depending on the input of the parameter it changes the output to digit1 
//and digit2. Is parameter is 1 then the output gets inverted 	
always @* begin
	digit1 = DIGIT_ENCODING[getdata/10];
	digit2 = DIGIT_ENCODING[getdata%10];
	if(INVERT_OUTPUT)begin
		digit1 = ~digit1;
		digit2 = ~digit2;
	end	
end



endmodule
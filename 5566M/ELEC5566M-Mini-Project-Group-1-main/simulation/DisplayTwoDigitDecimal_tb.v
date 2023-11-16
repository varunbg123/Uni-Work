/*
	2 Digit decimal on 2 Hex Displays Testbench
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 27/04/2023
	
	Description
	-----------
	This should take in the a decimal value 
	and displays it on 2 hex displays

*/


module DisplayTwoDigitDecimal_tb;

	// Inputs
	reg [5:0] getdata;
	parameter INVERT_OUTPUT = 1;
	
	// Outputs
	wire [6:0] digit1;
	wire [6:0] digit2;
	
	// Instantiate the Unit Under Test (UUT)
	DisplayTwoDigitDecimal #(
		.INVERT_OUTPUT(INVERT_OUTPUT)
	) uut (
		.getdata(getdata),
		.digit1(digit1),
		.digit2(digit2)
	);
	
	integer i;

initial begin
	// Initialize inputs
	getdata = 0;	
	
	// Wait for a few clock cycles before changing inputs
	#10;
	
	// Test cases for all two-digit numbers from 0 to 64
	for (i = 0; i < 64; i= i + 1) begin
		getdata = i;
		#10;
		$display("Input: %d, Output of Digit1: %b, Output of Digit2: %b", getdata, digit1, digit2);
		

	end
	
	// End simulation
	#10;
	$display("Test Complete\n");
end
	
endmodule
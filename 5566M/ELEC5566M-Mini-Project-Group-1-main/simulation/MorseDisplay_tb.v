/*
 * Morse Code Display test bench
 * -----------------------------
 * By: Aminul Hoque
 * Date: 11th April 2023
 *
 * Module Description
 * ------------------
 * The module is a test bench for the Morse Code Display.
 * This test bench performs a test of the first 38 possible inputs
 * of the Morse Code Display.
 */
// Timescale indicates unit of delays:
`timescale 1 ns/100 ps
// Test bench module declaration:
module MorseDisplay_tb;
	// Test Bench Generated Signal:
	reg  [5:0] morse_Code;
   // DUT Output Signals: 
	wire [6:0] hex5;
	wire [6:0] hex4;
	wire [6:0] hex3;
	wire [6:0] hex2;
	wire [6:0] hex1;
	// Device Under Test:
	MorseDisplay MorseDisplay_dut (
		.morse_Code ( morse_Code ),
		.hex5 		( hex5		 ),
		.hex4 		( hex4		 ),
		.hex3 		( hex3		 ),
		.hex2 		( hex2		 ),
		.hex1 		( hex1		 )
	);
	// Variables:
	integer i; // Loop counter.
	// Test Bench Logic:
	initial begin
		// Print to console the simulation start time:
		$display("%d ns\tSimulation Started",$time);
		// Print to console the input and the outputs:
		$monitor("%d ns\tmorse_Code=%d\thex5=%b\thex4=%b\thex3=%b\thex2=%b\thex1=%b",$time,morse_Code,hex5,hex4,hex3,hex2,hex1);
		for(i = 0; i < 38; i = i + 1) begin // Loop through first 38 possible inputs.
			morse_Code = i; // Set the value of morse_Code to the value of loop counter.
			#10; // Wait 10 units.
		end
		// Print to console the simulation finish time:
		$display("%d ns\tSimulation Finished",$time);
	end
endmodule

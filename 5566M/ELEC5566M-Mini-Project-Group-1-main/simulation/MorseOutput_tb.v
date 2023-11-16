/*
 * Morse Code Output test bench
 * ----------------------------
 * By: Aminul Hoque
 * Date: 1st May 2023
 *
 * Module Description
 * ------------------
 * The module is a test bench for the Morse Code Output.
 * This test bench performs a test of the 36 possible input of the
 * Morse Code Output: the numbers from 0 to 9 and the letters from
 * A to Z which are represented by the numbers from 10 to 35.
 * [To speed up the simulation change the local parameters for counter
 * in MorseOutput.v as follow: from 12499999 (250 ms) to 124 (2.5 μs) 
 * for DOT and SPACE, from 37499999 (750 ms) to 374 (7.5 μs) for DASH
 * and from 249999999 (5 s) to 2499 ( 50 μs) for END.]
 */
// Timescale indicates unit of delays:
`timescale 1 ns/100 ps
// Test bench module declaration:
module MorseOutput_tb;
	// Test Bench Generated Signals:
	reg  		  clock;
	reg  		  switch;
	reg  [5:0] morseCode;
   // DUT Output Signal: 
	wire		  morseOutput;
	// Device Under Test:
	MorseOutput MorseOutput_dut (
		.clock		 ( clock	    	),
		.switch		 ( switch		),
		.morseCode   ( morseCode   ),
		.morseOutput ( morseOutput	)
	);
	// Variable:
	integer i; // Loop counter.
	// Test Bench Logic:
	initial begin
		clock = 1'b0;
		// Print to console the simulation start time:
		$display("%d ns\tSimulation Started",$time);
		// Print to console the input and the output:
		$monitor("%d ns\tmorseCode=%d\tmorseOutput=%b",$time,morseCode,morseOutput);
		for(i = 0; i < 36; i = i + 1) begin // Loop through first 36 possible inputs.
			switch = 1'b0;
			#10; // Wait 10 units.
			switch = 1'b1;
			morseCode = i; // Set the value of morseCode to the value of loop counter.
			#50000; // Wait for morse code sequence to end.
		end
		// Print to console the simulation finish time:
		$display("%d ns\tSimulation Finished",$time);
		$stop;
	end
	// Generate clock signal:
	always begin
		#10; // Wait half clock period (10 ns).
		clock = ~clock;
	end
endmodule

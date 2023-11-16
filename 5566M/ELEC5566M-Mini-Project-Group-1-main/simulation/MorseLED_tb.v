/*
 * Morse Code Input LED test bench
 * -------------------------------
 * By: Aminul Hoque
 * Date: 25th April 2023
 *
 * Module Description
 * ------------------
 * The module is a test bench for the Morse Code Input LED.
 * This test bench performs a test of the 2 possible input
 * conditions of the Morse Code Input LED: the press of the
 * DOT push button and the press of the DASH push button.
 */
 // Timescale indicates unit of delays:
`timescale 1 ns/100 ps
// Test bench module declaration:
module MorseLED_tb;
	// Test Bench Generated Signals:
	reg  		  clock;
	reg  [1:0] pushButton;
   // DUT Output Signal: 
	wire		  LED;
	// Device Under Test:
	MorseLED MorseLED_dut (
		.clock		( clock		 ),
		.pushButton ( pushButton ),
		.LED			( LED			 )
	);
	// Test Bench Logic:
	initial begin
		clock = 1'b0;
		// Print to console the simulation start time:
		$display("%d ns\tSimulation Started",$time);
		// Print to console the input and the output:
		$monitor("%d ns\tpushButton=%b\tLED=%b",$time,pushButton,LED);
		pushButton = 2'b00; // No push button pressed.
		#100; // Wait 100 ns.
		pushButton = 2'b10; // DOT push button pressed.
		#100;
		pushButton = 2'b00;
		#250000000; // Wait 250 ms.
		pushButton = 2'b01; // DASH push button pressed.
		#100;
		pushButton = 2'b00;
		#750000000; // Wait 750 ms.
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

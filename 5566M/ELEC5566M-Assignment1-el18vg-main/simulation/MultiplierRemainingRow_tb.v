/*
	Multiplier Remaining Row Testbench
	-------------------------------
	By: Varun Gonsalves
	For: University of Leeds
	Date: 20/02/2023

	Description
	This is the testbench to check the Multiplier Remaining Row File.

*/

`timescale 1 ns/100 ps

module MultiplierRemainingRow_tb;

//Testbench Inputs
reg 		 	pp;
reg        	m;
reg 		  	q;
reg      	cIn;

//DUT outputs
wire 		  qOut;
wire       cOut;
wire       mOut;
wire       s;

//Connecting MultiplierRemainingRow inputs and outputs
MultiplierRemainingRow DUT(
	.pp(pp),
	.m(m),
	.q(q),
	.cIn(cIn),
	
	.qOut(qOut),
	.cOut(cOut),
	.mOut(mOut),
	.s(s)

);

//Some initial values to check if wiring is correct
initial begin
	pp = 1'b1; //change from 1'b0;
	m = 2'b11;
	q = 3'b110;
	cIn = 0;
	#10;
	pp = 1'b1; //change from 1'b0;
	m = 2'b11;
	q = 3'b011;
	cIn = 0;
	#10;

end

endmodule
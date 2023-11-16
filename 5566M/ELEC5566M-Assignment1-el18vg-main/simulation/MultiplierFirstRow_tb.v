/*
	Multiplier First Row Testbench
	-------------------------------
	By: Varun Gonsalves
	For: University of Leeds
	Date: 20/02/2023

	Description
	This is the testbench to check the Multiplier First Row File.

*/


`timescale 1 ns/100 ps

module MultiplierFirstRow_tb;

//Testbench Inputs
reg  [1:0] m;
reg  [2:0] q;
reg      cIn;
//DUT outputs
wire [1:0] qOut;
wire       cOut;
wire       mOut;
wire       s;

//DUT Connections

MultiplierFirstRow DUT(
	 .m(m),
    .q(q),
	 .cIn(cIn),
	 .qOut(qOut),
	 .cOut(cOut),
	 .mOut(mOut),
	 .s(s)

);

//some initial values to check if wiring is correct
initial begin
	m = 2'b11;
	q = 3'b110;
	cIn = 0;
	#10;
	m = 2'b11;
	q = 3'b011;
	cIn = 0;
	#10;


end

endmodule
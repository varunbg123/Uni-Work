/*
	Multiplier Top Testbench
	-------------------------------
	By: Varun Gonsalves
	For: University of Leeds
	Date: 20/02/2023

	Description
	This is the testbench to check the Multiplier.

*/

`timescale 1 ns/100 ps

module Multiplier2x3_tb;

reg  [1:0] m;
reg  [2:0] q;
    
wire [4:0] p;

//DUT Connections
Multiplier2x3 DUT(
	.m(m),
	.q(q),
	.p(p)
);

//integers and reg required. Integers are for the for loops used
// The reg is to store values to display and check that the modules are working as intended.
integer i;
integer j;
reg [4:0]truevalue = 0;
reg correct = 0;

initial begin
	
	$display("%d ns\tSimulation Started",$time);
	
	$monitor("%d ns\tm=%d\tq=%d\tp=%d\ttruevalue=%d\tCorrect=%d",$time,m,q,p,truevalue,correct);
	
	//This is 2 for Loops that will run through each values possible for m and q.
	//There are 4 values possible for m and there are 8 values possible for q.
	//There is also a truevalue that gets printed that can be checked to make sure the output is correct
	//Alongside with there is a flag that will highlight if the value is correct or not.
	for(i = 0; i < 4; i = i + 1)begin
		for(j = 0; j < 8; j = j + 1)begin
				m = i;
				q = j;
				#10;
				truevalue = m * q;
				if(p == truevalue)begin
					correct = 1;
				end else begin
					correct = 0;
				end
				#10;
		end
	end

	$display("%d ns\tSimulation Ended",$time);

end

endmodule
`timescale 1ns/1ps
/*
	Clock Divider Testbench
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 09/05/2023
	
	Description
	-----------
	This testbench is to test different clock frequency 
	that can be produced from the clock divider module

*/

module ClockDivider_tb;

reg clk;
wire divided_clk;

ClockDivider #(
	.DESIRED_FREQUENCY(10) // Set desired frequency to 10Hz
) clock_divider(
	.clk(clk),
	.divided_clk(divided_clk)
);


initial begin
	clk = 0;
	#5; // wait for 5ns
	$monitor("%d ns\t divided_clk = %b", $time, divided_clk);
	repeat(50_000_000)begin
	#10;
	clk = ~clk;

	end
end

endmodule
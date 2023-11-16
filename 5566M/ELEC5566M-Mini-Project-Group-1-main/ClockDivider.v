/*
	Clock Divider 
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 11/04/2023
	
	Description
	-----------
	This is a clock divider module that is parameterised.
	The inputs to it are the main clock the parameter of 
	the desired clock frequency. The ouput is the desired clock out.

*/

//takes the input and provides an output
module ClockDivider #(
	// this parameter is the value required. Default set to 10
	parameter DESIRED_FREQUENCY = 10
)(
	input clk,
	output reg divided_clk
);

//localparam divide_value = 1;
localparam CLK_FREQUENCY = 50_000_000;

// Counter value and the max count is used
// max count is the clk frequency/desired frequency divided by 2
integer countervalue = 0;
integer maxcount = 0;

// Set the initial values 
initial begin 
	divided_clk <= 0;
	maxcount = ((CLK_FREQUENCY/DESIRED_FREQUENCY))/2;

end


// this should increase and wait for
// the time and increase the value by 1
always @(posedge clk)begin
	countervalue <= countervalue + 1;
	// When the value is the max count the output clk can flip
	if(countervalue == maxcount)begin
		divided_clk <= ~divided_clk;
		countervalue <= 0;
	end
end


endmodule
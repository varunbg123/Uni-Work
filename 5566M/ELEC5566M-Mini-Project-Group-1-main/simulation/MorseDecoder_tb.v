/*
	Button Decoder Testbench
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 12/04/2023
	
	Description
	-----------
	This should take in the input from the buttons and then display them
	Into a data bit that can be passed into other modules

*/
`timescale 1ns/100ps

module MorseDecoder_tb;


reg clk;
reg [2:0]button;

wire led;

wire [5:0] data;

wire [5:0] storedata;

wire divided_clk;

parameter DESIRED_FREQUENCY = 10_000;

ClockDivider #(
	.DESIRED_FREQUENCY(DESIRED_FREQUENCY)
	)CLKDUT(
	.clk(clk),
	.divided_clk(divided_clk)
);

MorseDecoder DUT(
	.clk(clk),
	.button(button),
	.led(led),
	.data(data),
	.storedata(storedata)
);


integer count = 0;

integer clearcount = 0;

initial begin
	clk = 0;
	button = 3'b000;

	repeat(500000)begin
		#10;
		clk = ~clk;
	end
end


always @(posedge clk)begin
	count = count + 1;
	if (count == 10) begin
		button <= 3'b001;
	end 
	else if(count == 12) begin
		button <= 3'b010;
	end
	else if(count == 14) begin
		button <= 3'b001;
	end
	else if(count == 16) begin
		button <= 3'b010;
	end
	else if(count == 18) begin
		button <= 3'b010;
	end
	else if(count == 20) begin
		button <= 3'b001;
	end
	else if(count == 30) begin
		//count <= 0;
	end
	else if (count == 36) begin
		button <= 3'b001;
	end 
	else if(count == 38) begin
		button <= 3'b001;
	end
	else if(count == 40) begin
		button <= 3'b010;
	end
	else if(count == 42) begin
		button <= 3'b001;
	end
	else if(count == 44) begin
		button <= 3'b010;
	end
	else if(count == 46) begin
		button <= 3'b001;
	end
	else if(count == 56) begin
		count <= 0;
	end
	else begin
		button <= 3'b000;
		
	end

end


always @(posedge clk)begin
	clearcount = clearcount + 1;
	if((clearcount%30)== 0) begin
		button <= 3'b100;
		clearcount <= 0;
	end

end

endmodule
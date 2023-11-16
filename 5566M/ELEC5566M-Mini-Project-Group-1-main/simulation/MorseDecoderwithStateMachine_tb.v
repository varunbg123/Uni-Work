/*
	Button Decoder with State Machine Testbench
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 27/04/2023
	
	Description
	-----------
	This should take in the input from the buttons and then display them
	Into a data bit that can be passed into other modules with a state machine

*/

`timescale 1ns/100ps

module MorseDecoderwithStateMachine_tb;

//inputs
reg clk;
reg [2:0] button;

//outputs
wire divided_clk;
wire [2:0] state;
wire [2:0] next_state;

//button count
wire [4:0] buttonpresscount;
	
//data output
wire [5:0] data_out;
wire [5:0] post_data;
wire [5:0]getdata;

//segement to the hex display	
wire [6:0]segment;

// counter for the testing
integer count;


//wire used to turn on the system
wire dataconvertconfirm;

MorseDecoderwithStateMachine DUT(
	.clk(clk),
	.button(button),
	.divided_clk(divided_clk),
	.state(state),
	.next_state(next_state),
	.data_out(data_out),
	.segment(segment),
	.buttonpresscount(buttonpresscount),
	.dataconvertconfirm(dataconvertconfirm),
	.post_data(post_data),
	.getdata(getdata)

);


initial begin
	clk = 0;
	count = 0;
	button = 3'b111;

	repeat(10_000)begin
		#10;
		clk = ~clk;
	end
	

	
end

always @(posedge clk)begin
	count = count + 1;
	//This is the test for letter e
//	if(count == 10)begin
//		button <= 3'b110;
//	end else if(count == 11)begin
//		button <= 3'b111;
//	end else if(count == 12)begin
//		button <= 3'b011;
//	end else if (count == 13)begin
//		button <= 3'b111;
//	end else if(count == 20)begin
//		count <= 0;
//	end

	// The test for the number 3
		if(count == 10)begin
			button <= 3'b110;
		end else if(count == 25)begin
			button <= 3'b111;
		end else if(count == 40)begin
			button <= 3'b110;
		end else if (count == 55)begin
			button <= 3'b111;
		end else if (count == 70)begin
			button <= 3'b110;
		end else if (count == 85)begin
			button <= 3'b111;
		end else if (count == 100)begin
			button <= 3'b101;
		end else if (count == 115)begin
			button <= 3'b111;
		end else if (count == 130)begin
			button <= 3'b101;
		end else if (count == 145)begin
			button <= 3'b111;
		end else if (count == 160)begin
			button <= 3'b011;
		end else if (count == 175)begin
			button <= 3'b111;
		end else if(count == 200)begin
			count <= 0;
		end
		
	
		
	
end


endmodule
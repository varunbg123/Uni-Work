/*
	Button Decoder with a state machine
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 27/04/2023
	
	Description
	-----------
	This should take in the input from the buttons and then display them
	Into a data bit that can be passed into other modules with a state machine
*/

module MorseDecoderwithStateMachine(
	//inputs
	input clk,
	input [2:0]button,
	
	//outputs
	output wire divided_clk,
	output reg [2:0] state, 

	
	//data output
	output reg [5:0] data_out,
	
	//output to the 7 segment
	output wire [6:0]segment,


	//output reg for post data, this is the data sent to the other modules
	output wire [5:0] post_data,
	output reg [5:0]getdata,

	//flag to confirm conversion from the dataconverter module
	output reg dataconvertconfirm,
	

	//output to the hex
	output wire [6:0]digit1,
	output wire [6:0]digit2,
	
	//output to the morse to ascii code
	output reg morseReady
);
		
//parameter sets the frequency

parameter DESIRED_FREQUENCY = 10_000;

parameter BUTTON_FREQUENCY = 15;

// reg for when button 1 is pressed
reg button_0_pressed;
// reg for when button 2 is pressed
reg button_1_pressed;
// reg for when button 3 is pressed
reg button_2_pressed;

// reg for storing a big counter for button 1
reg [31:0] debounce_counter_0;
// reg for storing a big counter for button 2
reg [31:0] debounce_counter_1;
// reg for storing a big counter for button 3
reg [31:0] debounce_counter_2;

// register to activate the debounce counter - will block button presses.
reg debounce_active_0;
reg debounce_active_1;
reg debounce_active_2;


// button count 
reg [4:0] buttonpresscount;

//For the next state
reg [2:0] next_state;
	


//connections to the clock divider module

ClockDivider #(
	.DESIRED_FREQUENCY(DESIRED_FREQUENCY)
	)ClockGen(
	.clk(clk),
	.divided_clk(divided_clk)
);

// parameter for the Hex to 7 segment Display
parameter INVERT_OUTPUT = 1;

//Connections to the Module for the Hex 7 segment display
HexTo7Segment #(
	.INVERT_OUTPUT(INVERT_OUTPUT)
	)HexToSegment(
	.hex(buttonpresscount),
	.segment(segment)
);

//Connections for the Dataconverter Module
DataConverter DataConverter(
	.data_out(data_out),
	.buttoncount(buttonpresscount),
	.post_data(post_data),
	.dataconvertconfirm(dataconvertconfirm)
);


//to display the double digit
DisplayTwoDigitDecimal#(
	.INVERT_OUTPUT(INVERT_OUTPUT)
	)DisplayTwoDigitDecimal(
	.getdata(getdata),
	.digit1(digit1),
	.digit2(digit2)
);

//define the states parameter
parameter STATE_INPUT = 3'b001;
parameter STATE_CONVERT = 3'b010;
parameter STATE_SAVE = 3'b100;


// set the default values
initial begin
	data_out<=6'd0;
	getdata <=6'd0;
	//initial all the states to be from the starting state
	state[2:0] <= 3'd1;
	next_state[2:0] <= 3'd1;
	//Set the button press count to 0 as well
	buttonpresscount <= 5'd0;
	// the data confirm that comes in from the conversion module set to 0
	dataconvertconfirm <= 0;

	// the 3 counters for the button debouncing are set to 0
	debounce_counter_0 <= 32'd0;
	debounce_counter_1 <= 32'd0;
	debounce_counter_2 <= 32'd0;
	
	// the reg to active the button press are set to 0
	debounce_active_0 <= 1'b0;
	debounce_active_1 <= 1'b0;
	debounce_active_2 <= 1'b0;
	
	
	// reg for when button 1 is pressed
	button_0_pressed <=0;
	// reg for when button 2 is pressed
	button_1_pressed <= 0;
	// reg for when button 3 is pressed
	button_2_pressed <= 0;
		
end


/*
 * 3 States in state machine
 * The first is getting all the button presses
 * When button 3 is pressed the state machine will move to state 2 the conversion
 * when the data is converted then the the state machine will move to state 3 which 
 * is to clear the reg and also send the data to the other modules that require data
 *
*/
always @(posedge clk) begin
	case(state)
	 STATE_INPUT: begin
		morseReady <= 0;
		// If the first button is pressed
		if (button_0_pressed == 1'b1) begin
			buttonpresscount <= buttonpresscount + 1;
			data_out[5:0] <= {data_out[4:0], 1'b0};
			state = STATE_INPUT;
		end
		// If the second button is pressed	
		else if (button_1_pressed == 1'b1) begin
			buttonpresscount <= buttonpresscount + 1;
			data_out[5:0] <= {data_out[4:0], 1'b1};
			state = STATE_INPUT;
		end
		// If the Third Button is pressed
		else if (button_2_pressed) begin			
			dataconvertconfirm <= 1;
			state = STATE_CONVERT;
		end
		else begin 
			state = STATE_INPUT;
		end
	 end
	 STATE_CONVERT: begin
		getdata <= post_data;
		// logic for the convert state
		// when switch is high this will move to the next state
		if(!dataconvertconfirm)begin
			state = STATE_SAVE;

		end
		else if(dataconvertconfirm)begin
			//here get the data from the module converter
			dataconvertconfirm <= 0;

			state = STATE_CONVERT;
		end
		else begin
			state = STATE_CONVERT;
		end
	 end
	 STATE_SAVE: begin
		// logic for the save state
		morseReady <= 1;
		buttonpresscount <= 0;
		data_out <= data_out << 7;
		state = STATE_INPUT;
	 end
	endcase
end
	

always @(posedge clk) begin
	if (debounce_active_0 == 1'b1) begin
		// Increment the clock on each divided clock period
		debounce_counter_0 = debounce_counter_0 + 1'b1;
		// Internal if statement to set variables based on timings
		
		// If the debounce counter is greater than 30000000, then set
		// debounce_active low and clear the counter
		if (debounce_counter_0 > 30_000_000) begin
			debounce_active_0 = 1'b0;
			debounce_counter_0 = 32'd0;
		// If the debounce counter is greater than 1, then clear the button presses
		// repeat for other buttons
		end else if (debounce_counter_0 >= 1) begin
			button_0_pressed = 1'b0;
		end
	// Check button presses when debounce_active is low
	end else if(button[0] == 1'b0) begin
		debounce_active_0 = 1'b1;
		button_0_pressed = 1'b1;
	end 

	if (debounce_active_1 == 1'b1) begin
		// Increment the clock on each divided clock period
		debounce_counter_1 = debounce_counter_1 + 1'b1;
		// Internal if statement to set variables based on timings
		
		// If the debounce counter is greater than 30000000, then set
		// debounce_active low and clear the counter
		if (debounce_counter_1 > 30_000_000) begin
			debounce_active_1 = 1'b0;
			debounce_counter_1 = 32'd0;
		// If the debounce counter is greater than 1, then clear the button presses
		// repeat for other buttons
		end else if (debounce_counter_1 >= 1) begin
			button_1_pressed = 1'b0;
		end
	// Check button presses when debounce_active is low
	end else if(button[1] == 1'b0) begin
		debounce_active_1 = 1'b1;
		button_1_pressed = 1'b1;
	end 
	
	if (debounce_active_2 == 1'b1) begin
		// Increment the clock on each divided clock period
		debounce_counter_2 = debounce_counter_2 + 1'b1;
		// Internal if statement to set variables based on timings
		
		// If the debounce counter is greater than 30000000, then set
		// debounce_active low and clear the counter
		if (debounce_counter_2 > 30_000_000) begin
			debounce_active_2 = 1'b0;
			debounce_counter_2 = 32'd0;
		// If the debounce counter is greater than 1, then clear the button presses
		// repeat for other buttons
		end else if (debounce_counter_2 >= 1) begin
			button_2_pressed = 1'b0;
		end
	// Check button presses when debounce_active is low
	end else if(button[2] == 1'b0) begin
		debounce_active_2 = 1'b1;
		button_2_pressed = 1'b1;
	end 
		
end




endmodule

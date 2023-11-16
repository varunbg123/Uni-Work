/*
 * Morse Code Input LED
 * --------------------
 * By: Aminul Hoque
 * Date: 24th April 2023
 *
 * Module Description
 * ------------------
 * The module turns on the LED for 250 ms if the DOT push button
 * is pressed and if the DASH push button is pressed then it
 * turns on the LED for 750 ms.
 * The module has been built using a state machine.
 */
module MorseLED (
	// Declare input ports
   input				  clock,
	input		  [1:0] pushButton,
	// Declare output port
	output reg 		  LED
);
// Declare registers
reg [ 2:0] state;
reg [25:0] counter;
// Declare local parameters
localparam DOT  = 12499999; // 250 ms
localparam DASH = 37499999; // 750 ms
// State definitions
localparam OFF_STATE  = 2'b00;
localparam DOT_STATE  = 2'b01;
localparam DASH_STATE = 2'b10;
// State machine
always @ (posedge clock) begin	  
	case (state)
		OFF_STATE: begin // Define state OFF behaviour
			LED = 1'b0;
			counter <= 1'b0;
			if (pushButton == 2'b10) begin
				state <= DOT_STATE;
			end
			else if (pushButton == 2'b01) begin
				state <= DASH_STATE;
			end
		end
		DOT_STATE: begin // Define state DOT behaviour
			LED = 1'b1;
         if (counter == DOT) begin
				counter <= 1'b0;
				state <= OFF_STATE;
			end
			else begin
				counter <= counter + 1'b1;
				state <= DOT_STATE;
			end
		end
      DASH_STATE: begin // Define state DASH behaviour
			LED = 1'b1;
         if (counter == DASH) begin
				counter <= 1'b0;
				state <= OFF_STATE;
			end 
			else begin
				counter <= counter + 1'b1;
				state <= DASH_STATE;
			end
		end
      default: begin
			state <= OFF_STATE;
		end
	endcase
end
endmodule

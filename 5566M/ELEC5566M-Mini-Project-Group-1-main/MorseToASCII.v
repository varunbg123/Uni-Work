/*

MorseToASCII module

Converts the Morse digits into ASCII codes and checks validity.

Finn Beckitt-Marshall, April 2023

*/

module MorseToASCII #(
	// 36 possible Morse combinations, so make sure the register created is
	// wide enough for these.
	parameter	MORSE_WIDTH = $clog2(36)
)(
	// Morse code input
	input [MORSE_WIDTH - 1:0]	morse,
	
	// Character ready input
	input											morseReady,
	
	// Clock input
	input											clock,
	
	// ASCII code output
	output reg [6:0]					ascii,
	
	// Character ready output
	output reg								charReady
);

// Register to store whether the Morse value input is valid.
reg	morseValid;

// Set the outputs to their initial values
initial begin
	ascii <= 6'b0;
	charReady <= 1'b0;
	morseValid <= 1'b0;
end

// Always block
always @ (posedge clock) begin
	// If charReady is high, then set it low again
	if (charReady == 1'b1) begin
		charReady = 1'b0;
		morseValid = 1'b0;
	end
	// At the clock positive edge, check if the Morse code input is ready.
	// If it is, then check the value is valid.
	if (morseReady == 1'b1) begin
		// If the input is between 0 and 9, it is a numerical digit, so add 0x30
		// to the value.
		if (morse <= 9) begin
			ascii <= morse + 6'h30;
			morseValid = 1'b1;
		// If the input is between 10 and 35, add 0x37 to the value to move the
		// value into the upper case ASCII range.
		end else if (morse <= 35) begin
			ascii <= morse + 6'h37;
			morseValid = 1'b1;
		// If the input is equal to 36, it is a space, which is ASCII code 0x20.
		end else if (morse == 36) begin
			ascii <= 6'h20;
			morseValid = 1'b1;
		// If the input is out of range, then set the output to 0, and do not
		// toggle charReady.
		end else begin
			ascii <= 6'h0;
			morseValid = 1'b0;
		end
	end
	
	// If morseValid is 1'b1, then set charReady high for the next clock cycle.
	if (morseValid == 1'b1) begin
		charReady = 1'b1;
	end
end


endmodule

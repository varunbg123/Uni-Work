/*
CharacterHardware.v

Top-level module for implementing the character generator as a hardware test.

Finn Beckitt-Marshall, April 2023
*/

module CharacterHardware (
	// Clock input
	input									clock,
	// Global reset input
	input 								globalReset,
	// Application reset for debugging
	output								resetApp,
	
	// LT24 display interface
	output								LT24Wr_n,
	output								LT24Rd_n,
	output								LT24CS_n,
	output								LT24RS,
	output								LT24Reset_n,
	output	[15:0]				LT24Data,
	output								LT24LCDOn,
	
	// LCD clear input
	input									lcdClear,
	// Output to show the state machine outputs
	output [5:0]					stateOut,
	
	// ASCII character input
	input [6:0]							asciiIn,
	// Character ready input
	input									charReadyIn
);

wire lcd_clear_n;
wire global_reset_n;
wire divided_clock;
reg char_ready_n;
reg [24:0] debounce_counter;

not(lcd_clear_n, lcdClear);
not(global_reset_n, globalReset);

// Instantiate a local instance of CharacterSequential
CharacterSequential charSeq (
	.asciiChar 		(asciiIn				),
	.charReady		(char_ready_n		),
	.lcdClear			(lcd_clear_n	),
	.seqState			(stateOut[5:3]),
	.lcdState			(stateOut[2:0]),
	.clock				(clock				),
	.globalReset	(global_reset_n),
	.resetApp			(resetApp		  ),
	.LT24Wr_n			(LT24Wr_n			),
	.LT24Rd_n			(LT24Rd_n			),
	.LT24CS_n			(LT24CS_n			),
	.LT24RS				(LT24RS				),
	.LT24Reset_n	(LT24Reset_n	),
	.LT24Data			(LT24Data			),
	.LT24LCDOn		(LT24LCDOn		),
	.rowOffset		(5'b00000			),
	.colOffset		(6'b000000		)
	
);

initial begin
	char_ready_n <= 1'b0;
	debounce_counter <= 15'h0;
end

// To debounce the charReadyIn button, a debounce counter is used.
// 50MHz, debounce interval of 100ms - 50,000 ticks
always @ (posedge clock) begin
	if (charReadyIn == 1'b0 && debounce_counter <= 10) begin
		char_ready_n <= 1'b1;
		debounce_counter <= debounce_counter + 1'b1;
	end else if (charReadyIn == 1'b0 && debounce_counter <= 1000000) begin
		char_ready_n <= 1'b0;
		debounce_counter <= debounce_counter + 1'b1;
	end else begin
		char_ready_n <= 1'b0;
		debounce_counter <= 0;
	end
end

endmodule

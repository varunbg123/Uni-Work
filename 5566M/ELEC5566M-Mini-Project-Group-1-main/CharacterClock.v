/*
CharacterClock.v

Finn Beckitt-Marshall, March 2023
*/

module CharacterClock (
	input   [6:0]	character,
	input			clock,
	output	[63:0]	pixelData
);

// We want to subtract hex 20 from the character input to get the correct ROM
// address.
wire [6:0]  romAddress = character - 7'h20;

CharacterROM charROM (
	.address    (romAddress		),
	.clken	    (1'b1			),
	.clock	    (clock			),
	.q			(pixelData		)
);

endmodule

/*
MorseDecoder.v

Top level file for the ELEC5566M Mini Project - Group 1
Finn Beckitt-Marshall
Aminul Hoque
Varun Gonsalves

April 2023

*/

module MorseDecoder #(
	// PARAMETERS --------------------------------------------------------------
	// Number of 7 segment displays used - this calculates the appropriate
	// bus and wire size required.
	parameter	NUM_SEVEN_SEG = 5,
	
	// Width of bus - 7 * NUM_SEVEN_SEG
	parameter	WIDTH_SEVEN_SEG = 7 * NUM_SEVEN_SEG,
	
	// Number of Morse code digits - 36 is the default
	parameter	NUM_MORSE = 36
)(
	// INPUTS ------------------------------------------------------------------
	// Global clock input
	input				clock,
	
	// Global reset input
	input				globalReset,
	
	// Global clear button
	input				clear,
	
	// Size input
	input 			charSize,
	
	// DOT and DASH push buttons
	input	  [2:0]	inputButton,
	
	// Morse code input
	input	  [5:0]	morseInput,
	
	// Play Morse code signal switch
	input	  			switch,
	
	// OUTPUTS -----------------------------------------------------------------
	// Application reset for debugging
	output 			resetApp,
	
	// LT24 display interface
	output			LT24Wr_n,
	output			LT24Rd_n,
	output			LT24CS_n,
	output			LT24RS,
	output			LT24Reset_n,
	output [15:0]	LT24Data,
	output			LT24LCDOn,
	
	// MorseDisplay outputs
	output [6:0] 	morse1,
	output [6:0] 	morse2,
	output [6:0] 	morse3,
	output [6:0] 	morse4,
	output [6:0] 	morse5,
	
	// Morse input LED
	output 		 	inputLED,
	
	//output for MorseDecoder
	//output wire [5:0]postdata
	output [6:0] segment,

	// output to see the current dat output onto the leds
	output [5:0]data_out,
	
	// Play Morse code signal with the Buzzer
	output	  		sound,
	
	// Play Morse code signal with the LED
	output	      light

);

localparam WIDTH_MORSE = $clog2(NUM_MORSE);
localparam ASCII_SIZE = 7;

// Local register to store the Morse code value
wire	[WIDTH_MORSE - 1:0]	local_morse;
// Local register to store when Morse value is ready
wire											morse_ready;
// Local register to store the ASCII value
wire	[ASCII_SIZE - 1:0]	local_ascii;
// Local register to store when the ASCII character is ready
wire											char_ready;

// Create a local LCD clear wire which inverts the global clear input
wire lcd_clear;
not(lcd_clear, clear);


// Local wire to hold the state value
wire state;

// Instantiate a MorseToASCII module
MorseToASCII morseToASCII (
	.morse			(local_morse	),
	.morseReady	(morse_ready	),
	.clock			(clock				),
	.ascii			(local_ascii	),
	.charReady	(char_ready		)
);

// Instantiate a CharacterSequential module
CharacterSequential charSequential (
	.asciiChar		(local_ascii),
	.charReady		(char_ready	),
	.lcdClear			(lcd_clear 	),
	.charSize			(charSize		),
	.seqState			(						),
	.lcdState			(						),
	.clock				(clock			),
	.globalReset	(globalReset),
	.resetApp			(resetApp		),
	.LT24Wr_n			(LT24Wr_n		),
	.LT24Rd_n			(LT24Rd_n		),
	.LT24CS_n			(LT24CS_n		),
	.LT24RS				(LT24RS			),
	.LT24Reset_n	(LT24Reset_n),
	.LT24Data			(LT24Data		),
	.LT24LCDOn		(LT24LCDOn	),
	.rowOffset		(0					),
	.colOffset		(0					)
);

// Instantiate a MorseDisplay submodule
MorseDisplay morseDisplay (
	.morse_Code ( local_morse ),
	.hex5			( morse1		  ),
	.hex4			( morse2		  ),
	.hex3			( morse3		  ),
	.hex2			( morse4	     ),
	.hex1			( morse5		  )
);

// Instantiate a MorseLED submodule 
MorseLED morseLED (
	.clock 		( clock  	  ),
	.pushButton	( inputButton ),
	.LED			( inputLED	  )
);

// Instantiate the MorseDecoder Module State Machine

MorseDecoderwithStateMachine MorseDecoder(
	.clk(clock),
	.button(inputButton),
	.getdata(local_morse),
	.segment(segment),
	.state(state),
	.data_out(data_out),
	.morseReady(morse_ready)

);

// Instantiate a MorseOutput submodule for Buzzer
MorseOutput MorseOutputSound (
	.clock 		 ( clock 		),
	.switch		 ( switch 		),
	.morseCode	 ( morseInput	),
	.morseOutput ( sound			)
);

// Instantiate a MorseOutput submodule for LED
MorseOutput MorseOutputLight (
	.clock 		 ( clock 		),
	.switch		 ( switch 		),
	.morseCode	 ( morseInput	),
	.morseOutput ( light			)
);

endmodule

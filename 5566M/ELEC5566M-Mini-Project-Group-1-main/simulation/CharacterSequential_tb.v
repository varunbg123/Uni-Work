/*
CharacterSequential_tb.v

Finn Beckitt-Marshall, April 2023

Sequential testbench file.
*/

// Set the timescale
`timescale 1 ns/100 ps

module CharacterSequential_tb;

// Parameter declarations
localparam NUM_CYCLES = 5000000;     // Simulate 500,000 clock cycles
localparam CLOCK_FREQ = 50000000; 	// 50MHz clock

// DUT inputs
reg         clock;      // Clock
reg         reset;      // Reset
reg [7:0]   asciiChar;  // ASCII character to write
reg         lcdClear;   // LCD clear signal
reg         charReady;  // Character ready signal
reg [5:0]	rowOffset;	// Row offset
reg [4:0]	colOffset;	// Column offset
reg			charSize;	// Character size

// DUT outputs
wire        resetApp;
wire [2:0]	seqState;
wire [2:0]	lcdState;

// LT24 Display Interface
wire        LT24Wr_n;
wire        LT24Rd_n;
wire        LT24CS_n;
wire        LT24RS;
wire        LT24Reset_n;
wire [15:0]	LT24Data;
wire        LT24LCDOn;

CharacterSequential charSequential_dut (
	.asciiChar      (asciiChar  	),
    .charReady      (charReady  	),
    .lcdClear       (lcdClear   	),
	.seqState		(seqState		),
	.lcdState		(lcdState		),
	.charSize		(charSize		),
    .clock          (clock      	),
    .globalReset    (reset      	),
    .resetApp		(resetApp		),
	.LT24Wr_n		(LT24Wr_n		),
	.LT24Rd_n		(LT24Rd_n		),
	.LT24CS_n		(LT24CS_n		),
	.LT24RS			(LT24RS			),
	.LT24Reset_n	(LT24Reset_n	),
	.LT24Data		(LT24Data		),
	.LT24LCDOn		(LT24LCDOn		),
	.rowOffset		(rowOffset		),
	.colOffset		(colOffset		)
);

// Instantiate the LT24 functional model for simulation
LT24FunctionalModel #(
   .WIDTH  (240),
   .HEIGHT (320)
) displayModel (
   .LT24Wr_n    (LT24Wr_n		),
   .LT24Rd_n    (LT24Rd_n  		),
   .LT24CS_n    (LT24CS_n  		),
   .LT24RS      (LT24RS    		),
   .LT24Reset_n (LT24Reset_n 	),
   .LT24Data    (LT24Data    	),
   .LT24LCDOn   (LT24LCDOn   	)
);

// Testbench logic
// Reset the LCD and set everything to their initial values.
initial begin
	$display("Simulation started at %d ns.\n", $time);
	// Start in reset mode.
	reset = 1'b1;
	// Wait for 2 clock cycles
	repeat(2) @(posedge clock);
	// Clear the reset signal
	reset = 1'b0;
	// Wait until the resetApp signal is equal to 1'b0
	wait(resetApp === 1'b0);
	// Print a message stating that the LCD initialisation is now finished
	$display("Display initialisation finished at %d ns.\n", $time);	
	asciiChar 	= 0;
	lcdClear 	= 0;
	charReady	= 0;
	rowOffset	= 0;
	colOffset	= 0;
	charSize		= 0;
end

// Initialise the clock generator
initial begin
	clock = 1'b0;
	lcdClear = 1'b0;
end

// Convert the clock period to nanoseconds, and work out the half clock period
// from this.
real HALF_CLOCK_PERIOD = (1000000000.0 / $itor(CLOCK_FREQ)) / 2.0;

// Generate the clock
integer half_cycles = 0;

// Always block
always begin
	//$stop;
	// Generate the next clock half cycle
   	#(HALF_CLOCK_PERIOD);
   	clock = ~clock;
   	half_cycles = half_cycles + 1;
		
	// After 1000	half cycles have elapsed, set lcdClear high for 10 half
	// cycles. Repeat at 5000000 cycles.
	if ((half_cycles >= 1000 && half_cycles <= 1010) || (half_cycles >= 5000000 && half_cycles <= 5000010)) begin
		lcdClear <= 1'b1;
	end else begin
		lcdClear <= 1'b0;
	end

	// When the number of half cycles is between 3500000 and 3500010, then write
	// the letter A
	if (half_cycles >= 3500000 && half_cycles <= 3500010) begin
		asciiChar = 8'h41;	// 41 is hex ASCII code for A
		charReady = 1'b1;

	// When the number of half cycles is between 7500000 and 7500010, then write
	// the letter z at offset 15, 10 and set charSize to 1
	end else if (half_cycles >= 7500000 && half_cycles <= 7500010) begin
		asciiChar <= 8'h7A;	// 7A is hex ASCII code for z

		rowOffset = 15;
		colOffset = 10;
		charSize = 1'b1;
		charReady = 1'b1;
	end else begin
		charReady = 1'b0;
	end
	
	

	// If enough half cycles have been written, then reset the number of
	// half cycles and stop the simulation.
	if (half_cycles == (2 * NUM_CYCLES)) begin
		half_cycles = 0;
		$stop;
	end
end

always @ (posedge seqState[1]) begin
	$display("LCD cleared at %d ns.\n", $time);
end


endmodule

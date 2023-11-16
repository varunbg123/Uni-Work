/*
CharacterClock_tb.v

Testbench file for the CharacterClock module.

Finn Beckitt-Marshall, March 2023
*/

`timescale 1 ns/100 ps

module CharacterClock_tb;

// Parameter declarations
localparam NUM_CYCLES = 500;			// Simulate 50 clock cycles
localparam CLOCK_FREQ = 50000000; 	// 50MHz clock

// Testbench generated signals
reg 			clock;
reg [6:0]	character;

// Testbench output signals
wire [63:0]	pixeldata;

// Instantiate the DUT
CharacterClock characterClock_dut (
	.character	(character	),
	.clock		(clock		),
	.pixelData	(pixeldata	)
);

// Clock set
initial begin
	clock = 1'b0;		// Initialise clock to zero
	character = 7'h20;	// Initialise character to 20 (start of readable ASCII)
end

// Convert the clock period to nanoseconds and halve to get the half clock
// cycle period.
real HALF_CLOCK_PERIOD = (1e9 / $itor(CLOCK_FREQ)) / 2.0;

// Generate the clock
integer half_cycles = 0;
always begin
	#(HALF_CLOCK_PERIOD);	// Delay for half a clock period
	clock = ~clock;
	half_cycles = half_cycles + 1;
	
	// If an even number of half cycles has been done, then increment the
	// character register by 1.
	if ((half_cycles > 0) && (half_cycles % 2 == 0)) begin
		character = character + 1;
	end
	
	if (half_cycles == (2 * NUM_CYCLES)) begin
		half_cycles = 0;
		$stop;
	end
	
end


endmodule

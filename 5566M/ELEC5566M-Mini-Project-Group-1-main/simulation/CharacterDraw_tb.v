/*
CharacterDraw_tb.v

Simple testbench to test the CharacterDraw submodule.

Finn Beckitt-Marshall, March 2023
*/

// Set the testbench timescale
`timescale 1 ns/100 ps

module CharacterDraw_tb;

// Parameter declarations
localparam NUM_CYCLES = 1300;			// Simulate 50 clock cycles
localparam CLOCK_FREQ = 50000000; 	// 50MHz clock

// Testbench generated signals
reg	clock;
reg	reset;
reg charReady;
reg charSet; // Character set signal
reg [5:0]	lcdRow;
reg [5:0]	lcdCol;

// DUT outputs
wire			resetApp;
// LT24 Display Interface
wire        LT24Wr_n;
wire        LT24Rd_n;
wire        LT24CS_n;
wire        LT24RS;
wire        LT24Reset_n;
wire [15:0]	LT24Data;
wire        LT24LCDOn;
// Current state output
wire [1:0]	currentState;
wire [5:0] char_count_x;
wire [5:0] char_count_y;

// Instantiate the CharacterDraw.v
CharacterDraw characterDraw_DUT (
	.charData		(64'b0			),
	.clock			(clock			),
	.charReady	(charReady	),
	.globalReset	(reset			),
	.lcdRow			(lcdRow			),
	.lcdCol			(lcdCol			),
	.resetApp		(resetApp		),
	.LT24Wr_n		(LT24Wr_n		),
	.LT24Rd_n		(LT24Rd_n		),
	.LT24CS_n		(LT24CS_n		),
	.LT24RS			(LT24RS			),
	.LT24Reset_n	(LT24Reset_n	),
	.LT24Data		(LT24Data		),
	.LT24LCDOn		(LT24LCDOn		),
	.currentState (currentState	),
	.characterCountX (char_count_x),
	.characterCountY (char_count_y)
);

// Instantiate the LT24 functional model for simulation
LT24FunctionalModel #(
   .WIDTH  (240),
   .HEIGHT (320)
) displayModel (
   .LT24Wr_n    (LT24Wr_n		),
   .LT24Rd_n    (LT24Rd_n  	),
   .LT24CS_n    (LT24CS_n  	),
   .LT24RS      (LT24RS    	),
   .LT24Reset_n (LT24Reset_n 	),
   .LT24Data    (LT24Data    	),
   .LT24LCDOn   (LT24LCDOn   	)
	 
);

// Testbench logic
// Initial block to reset the LCD
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
	 
	 // Set the initial row and column to 0
	 lcdRow = 0;
	 lcdCol = 0;
	 
end

// Clock generator initialisation
initial begin
   clock = 1'b0;
	 charReady = 1'b1;
	 #10; // Wait 10 periods
	 charReady = 1'b0;
	 charSet = 1'b0;
	 // Set the initial row and column to 0
	 lcdRow = 0;
	 lcdCol = 0;
end

always @ (negedge resetApp) begin
		charReady = 1'b1;
		#100;
		charReady = 1'b0;
end

// Convert the clock period to nanoseconds, and work out the half clock period
// from this.
real HALF_CLOCK_PERIOD = (1000000000.0 / $itor(CLOCK_FREQ)) / 2.0;

// Generate the clock
integer half_cycles = 0;

always begin
	// Add an if statement to be printed when the LCD state is in its idle state.
	if (currentState == 2'b11) begin
		$display("%d ns - Currently idle...\n", $time);
	end
	
   // Generate the next clock half cycle
   #(HALF_CLOCK_PERIOD);
   clock = ~clock;
   half_cycles = half_cycles + 1;

   // If enough half cycles have been written, then reset the number of
   // half cycles and stop the simulation.
   if (half_cycles == (2 * NUM_CYCLES)) begin
       half_cycles = 0;
       $stop;
   end
end

endmodule

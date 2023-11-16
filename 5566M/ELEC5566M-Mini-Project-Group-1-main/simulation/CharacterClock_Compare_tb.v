/*
CharacterClock_Compare_tb.v

Testbench file for the CharacterClock module. This testbench file performs 10
test cases on the CharacterClock module, comparing the expected output to the
actual output, allowing automated testing.


Finn Beckitt-Marshall, March-May 2023
*/

`timescale 1 ns/100 ps

module CharacterClock_Compare_tb;

// Parameter declarations
localparam NUM_CYCLES = 500;			// Simulate 50 clock cycles
localparam CLOCK_FREQ = 50000000; 	// 50MHz clock

// Testbench generated signals
reg 		clock;
reg [6:0]	character;


// Testbench output signals
wire [63:0]	pixeldata;

// Arrays for test cases
reg [6:0]   character_test [9:0];
reg [63:0]  pixeldata_test [9:0];

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
    // Initialise the test cases into the arrays

    // Test case 0 - ASCII code 0 - null character
    // Expected ROM output - 0x0
    character_test[0] = 7'h0;
    pixeldata_test[0] = 64'h0;

    // Test case 1 - ASCII code 127 - Delete
    // Expected ROM output - 0x0
    character_test[1] = 7'h7F;
    pixeldata_test[1] = 64'h0;

    // Test case 2 - ASCII code 122 - z
    // Expected ROM output - 0x444c5c746444
    character_test[2] = 7'h7A;
    pixeldata_test[2] = 64'h444c5c746444;

    // Test case 3 - ASCII code 80 - P
    // Expected ROM output - 0xe1f11117f7f7f
    character_test[3] = 7'h50;
    pixeldata_test[3] = 64'he1f11117f7f7f;
    
    // Test case 4 - ASCII code 29 - Group seperator
    // Expected ROM output - 0x0
    character_test[4] = 7'h1D;
    pixeldata_test[4] = 64'h0;

    // Test case 5 - ASCII code 109 - m
    // Expected ROM output - 0x787c0c380c7c7c
    character_test[5] = 7'h6D;
    pixeldata_test[5] = 64'h787c0c380c7c7c;

    // Test case 6 - ASCII code 100 - d
    // Expected ROM output - 0x7f7f44447c38
    character_test[6] = 7'h64;
    pixeldata_test[6] = 64'h7f7f44447c38;

    // Test case 7 - ASCII code 53 - 5
    // Expected ROM output - 0x0
    character_test[7] = 7'h35;
    pixeldata_test[7] = 64'h387d7d45456727;

    // Test case 8 - ASCII code 20 - DC4
    // Expected ROM output - 0x0
    character_test[8] = 7'h14;
    pixeldata_test[8] = 64'h0;

    // Test case 9 - ASCII code 95 - _
    // Expected ROM output - 0x808080808080
    character_test[9] = 7'h5F;
    pixeldata_test[9] = 64'h808080808080;

    // Display a message indicating that testing has started
    $display("%d - Test started\n", $time);
end

// Convert the clock period to nanoseconds and halve to get the half clock
// cycle period.
real HALF_CLOCK_PERIOD = (1e9 / $itor(CLOCK_FREQ)) / 2.0;

// Generate the clock
integer half_cycles = 0;

// Test count integer
integer test_count = 0;

integer successful_tests = 0;

always begin
	#(HALF_CLOCK_PERIOD);	// Delay for half a clock period
	clock = ~clock;
	half_cycles = half_cycles + 1;
	
	// Every 10th half cycle, increment test_count to move to the next
    // value.
	if ((half_cycles > 10) && (half_cycles % 20 == 0)) begin
		test_count = test_count + 1;
        // Set the character value to character_test
        character = character_test[test_count];
	end

    // Perform the comparison every 2nd half cycle (i.e. 2, 12, 22 etc)
    if (half_cycles % 10 == 0) begin
        if (pixeldata_test[test_count] == pixeldata) begin
            $display("%d - Test successful - %s = %h\n", $time, character_test[test_count], pixeldata);
            successful_tests = successful_tests + 1'b1;
        end
    end
	
	if (half_cycles == (2 * NUM_CYCLES) || successful_tests >= 10) begin
        $display("%d - Test ended - %d successful tests\n", $time, successful_tests);
		half_cycles = 0;
		$stop;
	end
	
end


endmodule
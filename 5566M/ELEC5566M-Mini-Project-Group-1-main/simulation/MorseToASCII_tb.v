/*
MorseToASCII_tb.v

Testbench to exhaustively test the MorseToASCII module.

Finn Beckitt-Marshall, April 2023
*/

// Set the testbench timescale
`timescale 1 ns/100 ps

module MorseToASCII_tb;

// Parameter declarations
localparam NUM_CYCLES = 1000;	    // Simulate 1000 clock cycles
localparam CLOCK_FREQ = 50000000; 	// 50MHz clock

localparam NUM_TESTS = 37;

// Testbench generated signals
reg [5:0]   morse;
reg         morseReady;
reg         clock;

// Testbench outputs
wire [6:0]  ascii;
wire        charReady;

// Instantiate the MorseToASCII DUT
MorseToASCII morseToASCII_dut (
    .morse      (morse      ),
    .morseReady (morseReady ),
    .clock      (clock      ),
    .ascii      (ascii      ),
    .charReady  (charReady  )
);

// Testbench logic
// Initial block to reset/initialise the simulation
initial begin
    $display("Simulation started at %d ns.\n", $time);
    morse = 5'b0;
    morseReady = 1'b0;
    $display("Inputs initialised at %d ns.\n", $time);
end

// Clock generator initialisation
initial begin
   clock = 1'b0;
end

// Convert the clock period to nanoseconds, and work out the half clock period
// from this.
real HALF_CLOCK_PERIOD = (1000000000.0 / $itor(CLOCK_FREQ)) / 2.0;

// Generate the clock
integer half_cycles = 0;

// Integer value to compare the initial amount of half cycles
integer init_half_cycles = 0;

// Integer to store test count
integer test_count = 0;
// Integer to store tests passed
integer tests_passed = 0;

always begin
   // Generate the next clock half cycle
   #(HALF_CLOCK_PERIOD);
   clock = ~clock;
   half_cycles = half_cycles + 1;
   //morseReady = 1'b1;

   // If the number of half cycles is a multiple of 10, then
   // increment morse, and set morseReady high.
   if (half_cycles % 10 == 0) begin
      init_half_cycles = half_cycles;
      morse = morse + 1'b1;
      //morseReady = 1'b1;
   end else begin
      //morseReady = 1'b0;
   end

   // If half_cycles - init_half_cycles is less than 5, then hold morseReady
   // high.
   if (half_cycles - init_half_cycles < 5) begin
      morseReady = 1'b1;
   end else begin
      morseReady = 1'b0;
   end

   // When half_cycles - init_half_cycles == 2, then check the value.
   if (half_cycles - init_half_cycles == 2) begin
      test_count = test_count + 1;
      // Morse value between 0 and 9 - check that the ASCII code is within
      // the range
      if (morse <= 9) begin
         if (ascii - morse == 6'h30) begin
            $display("%d ns - PASS - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
            tests_passed = tests_passed + 1;
         end else begin
            $display("%d ns - FAIL - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
         end
      end else if (morse < 36) begin
         if (ascii - morse == 6'h37) begin
            $display("%d ns - PASS - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
            tests_passed = tests_passed + 1;
         end else begin
            $display("%d ns - FAIL - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
         end
      end else if (morse == 36) begin
         if (ascii == 6'h20) begin
            $display("%d ns - PASS - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
            tests_passed = tests_passed + 1;
         end else begin
            $display("%d ns - FAIL - Morse value = %h, ASCII value = %s\n", $time, morse, ascii);
         end
      end
   end

   // If enough half cycles have been written, then reset the number of
   // half cycles and stop the simulation.
   if (half_cycles == (2 * NUM_CYCLES) || (test_count >= NUM_TESTS)) begin
       if (test_count == tests_passed) begin
         $display("Success - all tests passed!\n");
       end else begin
         $display("One or more tests has failed.\n");
       end
       
       half_cycles = 0;
       $stop;
   end
end

endmodule

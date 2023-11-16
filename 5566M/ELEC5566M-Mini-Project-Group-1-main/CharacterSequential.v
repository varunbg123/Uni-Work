/*
CharacterSequential.v

Finn Beckitt-Marshall, April 2023

Sequentially writes characters to the LCD (i.e. will put the character in the
next allowable position).
*/

module CharacterSequential #(
	// Number of LCD rows - default is 40, as with a character set 8 pixels
	// high, and with a 320 pixel vertical resolution, this results in 40
	// rows.
	parameter NUM_ROWS = 6'd40,
	
	// Number of LCD rows when the character size is doubled
	parameter NUM_ROWS_DOUBLED = 6'd20,
	
	// Number of LCD columns - default is 30, as with a character set 8 pixels
	// wide, and with a 240 pixel horizontal resolution, this results in 30
	// columns.
	parameter NUM_COLS = 6'd30,
	
	// Number of LCD columns when the character size is doubled
	parameter NUM_COLS_DOUBLED = 6'd15,
	
	// LCD row bit width - uses the $clog2 macro to work out the correct width
	// required.
	parameter ROW_WIDTH = $clog2(NUM_ROWS),
	
	// LCD column bit width - uses the $clog2 macro to work out the correct 
	// width required.
	parameter COL_WIDTH = $clog2(NUM_COLS),
	
	// ASCII width - default is 7, as the extended ASCII set is not used, but
	// could be extended to 8 - the ROM would need to be extended to do this.
	parameter ASCII_WIDTH = 7,
	
	// Character ROM output width - default is 64, due to having 8x8
	// characters.
	parameter CHAR_ROM_WIDTH = 64
)(
	// ASCII code input
	input [(ASCII_WIDTH - 1):0]	asciiChar,
	
	// Character ready input - this should be set high when the character
	// at asciiChar is ready to write on the LCD, and then set low again.
	input									charReady,
	
	// LCD clear input - this is set high to clear the LCD, and takes
	// precedence over other inputs.
	input									lcdClear,
	
	// 2x size input - will make the characters twice the size, as they are
	// small at the moment.
	input									charSize,
	
	// CharacterSequential state machine output - used for debugging to view
	// the current state machine state.
	output reg [2:0] 					seqState,
	
	// CharacterDraw state machine output - used for debugging to view the
	// current state.
	output [2:0] 						lcdState,
	
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
	output [15:0]						LT24Data,
	output								LT24LCDOn,

	// Row and column offsets - these set the initial row and column offset
	// for the LCD. $clog2 is used to calculate the ceiling of log2 i.e.
	// the required width of these values.
	input [ROW_WIDTH - 1:0]		rowOffset,
	input [COL_WIDTH - 1:0] 	colOffset
);

// Local column position register
reg [COL_WIDTH - 1:0]			col;

// Local row position register
reg [ROW_WIDTH - 1:0]			row;

// Local character ROM data wire
wire [CHAR_ROM_WIDTH - 1:0] 	char_data;

// Local LCD clear finished wire
wire									lcd_clear_finished;

// Local character finished wire
wire									lcd_char_finished;

// Local character ready wire
reg									lcd_char_ready;

// Local register to store the current state
reg [2:0]							seq_state;

// States for the state machine
localparam	SEQ_INIT_STATE		= 3'b000;	// Initial state after reset
localparam	SEQ_CLEAR_STATE	= 3'b001; 	// LCD clear state
localparam	SEQ_WAIT_STATE		= 3'b010;	// Wait state - wait for a valid char
localparam	SEQ_WRITE_STATE	= 3'b011;	// Write the character on the LCD
localparam	SEQ_OFFSET_STATE 	= 3'b100;	// Set the new col/row locations

// Instantiate a local CharacterClock
CharacterClock charClock (
	.character 	(asciiChar	),
	.clock		(clock		),
	.pixelData	(char_data	)
);

// Instantiate a local CharacterDraw
CharacterDraw charDraw (
	.charData			(char_data				),
	.clock				(clock					),
	.charSize			(charSize				),
	.charReady			(lcd_char_ready		),
	.charFinished		(lcd_char_finished	),
	.globalReset		(globalReset			),
	.resetApp			(resetApp				),
	.lcdClear			(lcdClear				),
	.lcdClearFinished	(lcd_clear_finished	),
	.lcdRow				(row						),
	.lcdCol				(col						),
	.LT24Wr_n			(LT24Wr_n				),
	.LT24Rd_n			(LT24Rd_n				),
	.LT24CS_n			(LT24CS_n				),
	.LT24RS				(LT24RS					),
	.LT24Reset_n		(LT24Reset_n			),
	.LT24Data			(LT24Data				),
	.LT24LCDOn			(LT24LCDOn				),
	.currentState 		(lcdState				),
	.characterCountX 	( 							),
	.characterCountY 	( 							)
);

initial begin
	// Set the column and row values to zero initially.
	col 					= {(COL_WIDTH){1'b0}};
	row 					= {(ROW_WIDTH){1'b0}};
	lcd_char_ready 	<= 1'b0;

	// Set the initial value of the state machine to be in its initial state.
	seqState		<= SEQ_INIT_STATE;
end

always @ (*) begin
	seqState <= seq_state;
end

// State transition logic
always @ (posedge resetApp or posedge clock) begin
	
	// Actions to perform on reset
	if (resetApp) begin
		// Set the column and row values to be equal to zero.
		col 				<= {(COL_WIDTH){1'b0}};
		row 				<= {(ROW_WIDTH){1'b0}};
		lcd_char_ready <= 1'b0;
		// Set the initial value of the state machine to be in its initial state.
		seq_state 		<= SEQ_INIT_STATE;
	
	end else begin
		// Use a case statement to go through all of the possible states.
		case (seq_state)
			
			SEQ_INIT_STATE: begin
				// Move straight into the wait state
				seq_state <= SEQ_WAIT_STATE;
			end
			
			SEQ_WAIT_STATE: begin
				// If charReady is high, then move into the write state.
				if (lcdClear == 1'b1) begin
					seq_state <= SEQ_CLEAR_STATE;
					row = rowOffset;
					col = colOffset;
				end else if (charReady == 1'b1) begin
					lcd_char_ready <= 1'b1;

					seq_state <= SEQ_WRITE_STATE;
				end else begin
					seq_state <= SEQ_WAIT_STATE;
				end
			end
			
			SEQ_WRITE_STATE: begin
				// Once the character has finished writing, then move into the
				// offset state.
				lcd_char_ready <= 1'b0;
				if (lcd_char_finished == 1'b1 && charReady == 1'b0) begin
					
					seq_state <= SEQ_OFFSET_STATE;
				end
			end
			
			SEQ_OFFSET_STATE: begin
				col = col + 1'b1;
				// Calculate the new column and row offsets, and then move into
				// the wait state
				// If the column is equal to NUM_COLS, then set the column back
				// to zero and increment the row.
				if (col == NUM_COLS || (col == NUM_COLS_DOUBLED && charSize == 1'b1)) begin
					col = {(COL_WIDTH){1'b0}};
					row = row + 1'b1;
				end
					
				if (row == NUM_ROWS || (row == NUM_ROWS_DOUBLED && charSize == 1'b1)) begin
					seq_state <= SEQ_CLEAR_STATE;
				end else begin
					seq_state <= SEQ_WAIT_STATE;
				end
			end
			
			SEQ_CLEAR_STATE: begin
				// When the LCD has finished clearing, then move into the wait
				// state.
				if (lcd_clear_finished == 1'b1) begin
				
					seq_state <= SEQ_WAIT_STATE;
				end else begin
					seq_state <= SEQ_CLEAR_STATE;
				end
			end
		endcase
	end
end

endmodule

/*
CharacterDraw.v

Verilog module to draw characters onto the LCD display.

Finn Beckitt-Marshall, March 2023
*/

module CharacterDraw #(
	// Character width (default 8 pixels)
	parameter	CHAR_WIDTH 	= 8,
	
	// Character height (default 8 pixels)
	parameter	CHAR_HEIGHT	= 8,
	
	// Number of pixels per character (default CHAR_WIDTH * CHAR_HEIGHT = 64)
	parameter	CHAR_PIXELS = CHAR_WIDTH * CHAR_HEIGHT,
	
	// LCD rows (default 40)
	parameter	LCD_ROWS 	= 40,
	
	// LCD columns (default 30)
	parameter	LCD_COLS		= 30,
	
	// LCD row bit width - uses the $clog2 macro to work out the correct width
	// required.
	parameter 	ROW_WIDTH 	= $clog2(LCD_ROWS),
	
	// LCD column bit width - uses the $clog2 macro to work out the correct 
	// width required.
	parameter 	COL_WIDTH 	= $clog2(LCD_COLS),
	
	// LCD vertical resolution (default 320)
	parameter	LCD_V_RES	= 320,
	
	// LCD horizontal resolution (default 240)
	parameter	LCD_H_RES	= 240,
	
	// Width of the register needed to store vertical pixel locations
	parameter	LCD_V_WIDTH = $clog2(LCD_V_RES),
	
	// Width of the register needed to store vertical pixel locations
	parameter	LCD_H_WIDTH = $clog2(LCD_H_RES)
	
) (
	// Pixel data input of width CHAR_PIXELS
	input	[(CHAR_PIXELS - 1):0]	charData,
	
	// Character ready input
	input									charReady,
	
	// Character finished output
	output reg							charFinished,
	
	// Clock input
	input									clock,
	
	// Character size input
	input									charSize,
	
	// Row and column inputs to set the row or column where the character will
	// be drawn
	input [ROW_WIDTH - 1:0]			lcdRow,
	input [COL_WIDTH - 1:0]			lcdCol,
	
	// Global reset input
	input 								globalReset,
	
	// Application reset for debugging
	output								resetApp,
	
	// LCD clear input - when set, will clear the LCD, overriding everything
	// else.
	input									lcdClear,
	
	// Output when LCD has finished clearing - set high.
	output reg							lcdClearFinished,
	
	// LT24 display interface
	output								LT24Wr_n,
	output								LT24Rd_n,
	output								LT24CS_n,
	output								LT24RS,
	output								LT24Reset_n,
	output [15:0]						LT24Data,
	output								LT24LCDOn,
	
	// Current state output
	output reg [2:0]					currentState,
	output reg [COL_WIDTH - 1:0] 	characterCountX,
	output reg [ROW_WIDTH - 1:0] 	characterCountY
);

// Local registers
reg [LCD_H_WIDTH - 1:0]	xAddr;
reg [LCD_V_WIDTH - 1:0]	yAddr;
reg [15:0]					pixelData;
reg							pixelWrite;
reg [LCD_H_WIDTH - 1:0]	clear_count_x;
reg [LCD_V_WIDTH - 1:0]	clear_count_y;

// Local wires
wire pixelReady;

// Local parameters for the LCD display
localparam	LCD_WIDTH	= 240;
localparam	LCD_HEIGHT	= 320;

// Instantiate the LT24Display submodule here
LT24Display #(
	.WIDTH			(LCD_H_RES		),
	.HEIGHT			(LCD_V_RES		),
	.CLOCK_FREQ		(50000000		)
) display (
	// Clock and reset inputs
	.clock			(clock			),
	.globalReset	(globalReset	),
	// Reset for the user logic
	.resetApp		(resetApp		),
	// Pixel interface
	.xAddr			(xAddr			),
	.yAddr			(yAddr			),
	.pixelData		(pixelData		),
	.pixelWrite		(pixelWrite		),
	.pixelReady		(pixelReady		),
	// Use the pixel addressing mode of the LCD
	.pixelRawMode	(1'b0				),
	// Command interface is not used
	.cmdData			(8'b0				),
	.cmdWrite		(1'b0				),
	.cmdDone			(1'b0				),
	.cmdReady		(					),
	// Display connections
	.LT24Wr_n		(LT24Wr_n		),
	.LT24Rd_n		(LT24Rd_n		),
	.LT24CS_n		(LT24CS_n		),
	.LT24RS			(LT24RS			),
	.LT24Reset_n	(LT24Reset_n	),
	.LT24Data		(LT24Data		),
	.LT24LCDOn		(LT24LCDOn		)
);

// State parameters for the internal state machine
localparam	LCD_WAIT_STATE 		= 3'b000;
localparam	LCD_WRITE_STATE 		= 3'b001;
localparam	LCD_X_Y_CHECK_STATE	= 3'b010;
localparam	LCD_IDLE_STATE			= 3'b011;
localparam	LCD_CLEAR_STATE		= 3'b100;

// Register to store the current state
reg [2:0]	lcd_state;

// Register to store the count for the current pixel - this is because when
// when charSize is high, four pixels need to be drawn rather than 1.
reg [2:0]	pixel_count;

// Initial block to set the initial variable states
initial begin
	lcd_state 			<= LCD_WAIT_STATE;
	characterCountX 	<= {(COL_WIDTH){1'b0}};
	characterCountY 	<= {(ROW_WIDTH){1'b0}};
	clear_count_x 		<= {(LCD_H_WIDTH){1'b0}};
	clear_count_y 		<= {(LCD_V_WIDTH){1'b0}};
	pixelData 			<= 16'b0;
	lcdClearFinished 	<= 1'b1; // Set to 1 on reset, as lcdClear has not been called
	pixel_count <= 3'b0;
end

// State transition logic
always @ (posedge clock or posedge resetApp) begin
	currentState = lcd_state;
	if (resetApp) begin
		// When reset, set the state to LCD_WAIT_STATE.
		lcd_state <=	LCD_IDLE_STATE;
		// Reset the x and y values
		characterCountX	= {(COL_WIDTH){1'b0}};
		characterCountY	= {(ROW_WIDTH){1'b0}};
		lcdClearFinished 	<= 1'b1;
	end else begin
		if (lcdClear == 1'b1) begin
			// Reset clear_count_x and y
			clear_count_x 		<= {(LCD_H_WIDTH){1'b0}};
			clear_count_y 		<= {(LCD_V_WIDTH){1'b0}};
			lcdClearFinished 	<= 1'b0;
		end
		// Use a case loop to set the states
		case (lcd_state) 
			
			LCD_WAIT_STATE: begin
				pixelWrite <= 1'b1;
				if (pixelReady == 1'b1) begin
					if (lcdClearFinished == 1'b0) begin
						lcd_state <= LCD_CLEAR_STATE;
					end else begin 
						lcd_state <= LCD_WRITE_STATE;
					end
				end
			end
			
			LCD_WRITE_STATE: begin
				charFinished = 1'b0;
				if (pixelReady == 1'b0) begin
					if (charSize == 1'b1 && pixel_count <= 3'd3) begin
						
						pixel_count = pixel_count + 1'b1;
						pixelWrite <= 1'b0;
						lcd_state <= LCD_WAIT_STATE;
					end
					pixel_count <= 3'd0;
					lcd_state <= LCD_X_Y_CHECK_STATE;
				end
			end
			
			LCD_X_Y_CHECK_STATE: begin
				pixelWrite <= 1'b0;
				// If the final X character has been written, then either shift to next
				// line, or move into the idle state.
				if (characterCountX == (CHAR_WIDTH - 1)) begin
					if (characterCountY == (CHAR_HEIGHT - 1)) begin
						lcd_state = LCD_IDLE_STATE;
						charFinished = 1'b1;
					end else begin
						characterCountX = {(COL_WIDTH){1'b0}};
						characterCountY = characterCountY + 1'b1;						
						lcd_state = LCD_WAIT_STATE;
					end
				end else begin
					characterCountX = characterCountX + 1'b1;
					lcd_state = LCD_WAIT_STATE;
				end
				
			end
			
			LCD_IDLE_STATE: begin
				if (charReady == 1'b1 || lcdClear == 1'b1) begin
					lcd_state = LCD_WAIT_STATE;
				end
				// Reset the x and y values
				characterCountX <= {(COL_WIDTH){1'b0}};
				characterCountY <= {(ROW_WIDTH){1'b0}};
			end
			
			LCD_CLEAR_STATE: begin
				// Increment clear_count_x
				clear_count_x <= clear_count_x + 1'b1;
				// Check if it is at the end of the line - if so, then increment
				// y, and set clear_count_x to zero. However, if the final line
				// has been cleared, then move to the LCD idle state, and set
				// lcdClearFinished to 1.
				if (clear_count_x >= LCD_WIDTH) begin
					if (clear_count_y < LCD_HEIGHT) begin
						clear_count_x <= {(LCD_H_WIDTH){1'b0}};
						clear_count_y <= clear_count_y + 1'b1;
						lcd_state = LCD_WAIT_STATE;
					end else begin
						lcdClearFinished <= 1'b1;
						pixelWrite 	<= 1'b0;
						lcd_state 	<= LCD_IDLE_STATE;
					end
				end else begin
					lcd_state <= LCD_WAIT_STATE;
				end
			end
			
			default: lcd_state <= LCD_IDLE_STATE;
		endcase
	end
end

// Always block for initially setting the pixel at location (0, 0) to white.
always @ (posedge clock or posedge resetApp) begin
	if (resetApp) begin
		pixelData 	<= 16'b0;
		xAddr			<= {(LCD_H_WIDTH){1'b0}};
		yAddr			<= {(LCD_V_WIDTH){1'b0}};
	end else if (lcd_state == LCD_WRITE_STATE) begin
		//xAddr = characterCountX;
		//yAddr = characterCountY;
		if (charSize == 1'b1) begin
			xAddr	= 2'd2 * characterCountY + pixel_count[0] + (2'd2 * CHAR_WIDTH[3:0] * lcdCol);
			yAddr	= 2'd2 * characterCountX + pixel_count[1] + (2'd2 * CHAR_HEIGHT[3:0] * lcdRow);
		end else begin
			xAddr	= characterCountY + (CHAR_WIDTH[3:0] * lcdCol);
			yAddr	= characterCountX + (CHAR_HEIGHT[3:0] * lcdRow);
		end
		// Write a white pixel to that location if the value of charData = 1, else
		// write a black pixel.
		if (charData[CHAR_WIDTH * characterCountY + characterCountX] == 1'b1) begin
			pixelData	<= 16'hFFFF;
		end else begin
			pixelData	<= 16'h0000;
		end
	end else if (lcd_state == LCD_CLEAR_STATE) begin
		// Write a black pixel to the LCD
		xAddr = clear_count_x;
		yAddr = clear_count_y;
		pixelData	<= 16'h0000;
		
		
	end
end

endmodule

/*
 * Morse Code Display
 * ------------------
 * By: Aminul Hoque
 * Date: 11th April 2023
 *
 * Module Description
 * ------------------
 * The module displays the Morse code on the 7-segment Displays
 * of the DE1-SoC Board.
 * The module has been built using case statements.
 */
module MorseDisplay (
	// Declare input port:
   input			[5:0] morse_Code,
	// Declare output ports:
   output reg	[6:0] hex5,
	output reg	[6:0] hex4,
	output reg	[6:0] hex3,
	output reg	[6:0] hex2,
	output reg	[6:0] hex1
);
// Declare local parameters
localparam DOT  = 7'b0100011;
localparam DASH = 7'b1110111;
localparam OFF  = 7'b1111111;
localparam E    = 7'b0000110;
localparam R    = 7'b0101111;
// Declare case statements
always @ * begin
	if (morse_Code > 36) begin
		hex5 = E; 
		hex4 = R; 
		hex3 = R; 
		hex2 = DOT; 
		hex1 = R;
	end 
	else begin
	case ( morse_Code )
		6'd0: begin // Displays 0 in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = DASH;
		end
		6'd1: begin // Displays 1 in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = DASH;
		end
		6'd2: begin // Displays 2 in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = DASH;
		end
		6'd3: begin // Displays 3 in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DASH; 
			hex1 = DASH;
		end
		6'd4: begin // Displays 4 in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = DASH;
		end
		6'd5: begin // Displays 5 in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = DOT;
		end
		6'd6: begin // Displays 6 in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = DOT;
		end
		5'd7: begin // Displays 7 in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = DOT;
		end
		6'd8: begin // Displays 8 in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DOT; 
			hex1 = DOT;
		end
		6'd9: begin // Displays 9 in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = DOT;
		end
		6'd10: begin // Displays A in Morse code
			hex5 = DOT; 
			hex4 = DASH;
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd11: begin // Displays B in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd12: begin // Displays C in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd13: begin // Displays D in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd14: begin // Displays E in Morse code
			hex5 = DOT; 
			hex4 = OFF;
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd15: begin // Displays F in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd16: begin // Displays G in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd17: begin // Displays H in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd18: begin // Displays I in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd19: begin // Displays J in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = OFF;
		end
		6'd20: begin // Displays K in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd21: begin // Displays L in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd22: begin // Displays M in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd23: begin // Displays N in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd24: begin // Displays O in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd25: begin // Displays P in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = DOT; 
			hex1 = OFF;
		end
		6'd26: begin // Displays Q in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = DASH; 
			hex1 = OFF;
		end
		6'd27: begin // Displays R in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd28: begin // Displays S in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd29: begin // Displays T in Morse code
			hex5 = DASH;
			hex4 = OFF;
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd30: begin // Displays U in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd31: begin // Displays V in Morse code
			hex5 = DOT; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DASH; 
			hex1 = OFF;
		end
		6'd32: begin // Displays W in Morse code
			hex5 = DOT; 
			hex4 = DASH; 
			hex3 = DASH; 
			hex2 = OFF;
			hex1 = OFF;
		end
		6'd33: begin // Displays X in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DOT; 
			hex2 = DASH; 
			hex1 = OFF;
		end
		6'd34: begin // Displays Y in Morse code
			hex5 = DASH; 
			hex4 = DOT; 
			hex3 = DASH; 
			hex2 = DASH; 
			hex1 = OFF;
		end
		6'd35: begin // Displays Z in Morse code
			hex5 = DASH; 
			hex4 = DASH; 
			hex3 = DOT; 
			hex2 = DOT; 
			hex1 = OFF;
		end	
		default: begin // Turns off the Displays
			hex5 = OFF;
			hex4 = OFF;
			hex3 = OFF;
			hex2 = OFF;
			hex1 = OFF;
		end
	endcase
	end
end
endmodule

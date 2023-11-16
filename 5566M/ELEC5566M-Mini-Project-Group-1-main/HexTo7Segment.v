 /*
 *  7 Segment Display
 * ----------------------------
 * By: Varun Gonsalves
 * For: University of Leeds
 * Date: 31/01/2023
 *
 * Description
 * ------------
 * Converting Hex to one 7 segment display
 *
 */
 
 
 
 module HexTo7Segment#(
		
		parameter INVERT_OUTPUT = 1
	
 
	)(
		//Declare input and output
		// As it is procedural the segment is declared as a reg. 
		// This eliminates the need for a wire.
		input [3:0]hex,
		output reg [6:0]segment
		
 );
 

 
always @ * begin
	case(hex)
		//case 1 when input is 0, segment turned on are 0,1,2,3,4,5
		4'b0000: segment = 7'b0111111;
		
		//case 2 when input is 1, segment turned on are 1,2
		4'b0001: segment = 7'b0000110;
		
		//case 3 when input is 2, segment turned on are 0,1,3,4,6
		4'b0010: segment = 7'b1011011;
		
		//case 4 when input is 3, segment turned on are 0,1,2,3,6
		4'b0011: segment = 7'b1001111;
		
		//case 5 when input is 4, segment turned on are 1,2,5,6
		4'b0100: segment = 7'b1100110;
		
		//case 6 when input is 5, segment turned on are 0,2,3,5,6
		4'b0101: segment = 7'b1101101;
		
		//case 7 when input is 6, segment turned on are 0,2,3,4,5,6
		4'b0110: segment = 7'b1111101;
		
		//case 8 when input is 7, segment turned on are 0,1,2
		4'b0111: segment = 7'b0000111;
		
		//case 9 when input is 8, segment turned on are 0,1,2,3,4,5,6
		4'b1000: segment = 7'b1111111;
		
		//case 10 when input is 9, segment turned on are 0,1,2,5,6
		4'b1001: segment = 7'b1100111;
		
		//case 11 when input is a, segment turned on are 0,1,2,4,5,6
		4'b1010: segment = 7'b1110111;
		
		//case 12 when input is b, segment turned on are 2,3,4,5,6
		4'b1011: segment = 7'b1111100;
		
		//case 13 when input is c, segment turned on are 0,3,4,5
		4'b1100: segment = 7'b0111001;
		
		//case 14 when input is d, segment turned on are 1,2,3,4,6
		4'b1101: segment = 7'b1011110;
		
		//case 15 when input is e, segment turned on are 0,3,4,5,6
		4'b1110: segment = 7'b1111001;
		
		//case 16 when input is f, segment turned on are 0,4,5,6
		4'b1111: segment = 7'b1110001;
	
		//default incase the cases break and need to reset.
		default: segment = 7'b0000000;
	endcase
		
	if (INVERT_OUTPUT == 1)begin
		segment = ~segment;
	end
	
end


endmodule
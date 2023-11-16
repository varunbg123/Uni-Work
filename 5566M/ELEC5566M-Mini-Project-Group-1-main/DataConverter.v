/*
	Converter Module
	----------------
	By: Varun Gonsalves
	For: University of Leeds Group Project
	Date: 28/04/2023
	
	Description
	-----------
	This will take in 2 values, data and the number of button presses

*/

module DataConverter(
	input [5:0]data_out,
	input [4:0]buttoncount,
	output reg [5:0]post_data,
	input dataconvertconfirm
);

initial begin
	post_data <= 6'd0;
end


always @ * begin
	if(dataconvertconfirm)begin
			if(buttoncount == 0)begin
				post_data <= 36;
			end
			else if(buttoncount == 1)begin
				//E
				if(data_out[0] == 0)begin
					post_data <= 14;
				end
				//T
				else if(data_out[0] == 1)begin
					post_data <= 29;
				end
			end
			else if(buttoncount == 2)begin
				//A
				if(data_out[1:0] == 2'b01)begin
					post_data <= 10;
				end
				//I
				else if(data_out[1:0] == 2'b00)begin
					post_data <= 18;
				end
				//M
				else if(data_out[1:0] == 2'b11)begin
					post_data <= 22;
				end
				//N
				else if(data_out[1:0] == 2'b10)begin
					post_data <= 23;
				end
			end
			else if (buttoncount == 3)begin
				// S
				if(data_out[2:0] == 3'b000)begin
					post_data <= 28;
				end
				// U
				else if(data_out[2:0] == 3'b001)begin
					post_data <= 30;
				end
				// R
				else if(data_out[2:0] == 3'b010)begin
					post_data <= 27;
				end
				// W
				else if(data_out[2:0] == 3'b011)begin
					post_data <= 32;
				end
				// D
				else if(data_out[2:0] == 3'b100)begin
					post_data <= 13;
				end
				// K
				else if(data_out[2:0] == 3'b101)begin
					post_data <= 20;
				end
				// G
				else if(data_out[2:0] == 3'b110)begin
					post_data <= 16;
				end
				// O
				else if(data_out[2:0] == 3'b111)begin
					post_data <= 24;
				end
			end
			else if (buttoncount == 4)begin
				// H
				if(data_out[3:0] == 4'b0000)begin
					post_data <= 17;
				end
				// V
				else if(data_out[3:0] == 4'b0001)begin
					post_data <= 31;
				end
				// F
				else if(data_out[3:0] == 4'b0010)begin
					post_data <= 15;
				end
				// L
				else if(data_out[3:0] == 4'b0100)begin
					post_data <= 21;
				end
				// P
				else if(data_out[3:0] == 4'b0110)begin
					post_data <= 25;
				end
				// J
				else if(data_out[3:0] == 4'b0111)begin
					post_data <= 19;
				end
				// B
				else if(data_out[3:0] == 4'b1000)begin
					post_data <= 11;
				end
				// X
				else if(data_out[3:0] == 4'b1001)begin
					post_data <= 33;
				end
				// C
				else if(data_out[3:0] == 4'b1010)begin
					post_data <= 12;
				end
				// Y
				else if(data_out[3:0] == 4'b1011)begin
					post_data <= 34;
				end
				// Z
				else if(data_out[3:0] == 4'b1100)begin
					post_data <= 35;
				end
				// Q
				else if(data_out[3:0] == 4'b1101)begin
					post_data <= 26;
				end
				else begin
					post_data <= 40;
				end		
			end
			else if (buttoncount == 5)begin
				// 1
				if(data_out[4:0] == 5'b01111)begin
					post_data <= 1;
				end
				// 2
				else if(data_out[4:0] == 5'b00111)begin
					post_data <= 2;
				end
				// 3
				else if(data_out[4:0] == 5'b00011)begin
					post_data <= 3;
				end
				// 4
				else if(data_out[4:0] == 5'b00001)begin
					post_data <= 4;
				end
				// 5
				else if(data_out[4:0] == 5'b00000)begin
					post_data <= 5;
				end
				// 6
				else if(data_out[4:0] == 5'b10000)begin
					post_data <= 6;
				end
				// 7
				else if(data_out[4:0] == 5'b11000)begin
					post_data <= 7;
				end
				// 8
				else if(data_out[4:0] == 5'b11100)begin
					post_data <= 8;
				end
				// 9
				else if(data_out[4:0] == 5'b11110)begin
					post_data <= 9;
				end
				// 0
				else if(data_out[4:0] == 5'b11111)begin
					post_data <= 0;
				end
				else begin
					post_data <= 40;
				end
			end
			else begin
				post_data <= 40;
			end
	end else begin
		// do nothing
	end
end

endmodule
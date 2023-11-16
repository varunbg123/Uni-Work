/*
 * Morse Code Output
 * -----------------
 * By: Aminul Hoque
 * Date: 29th April 2023
 *
 * Module Description
 * ------------------
 * The module takes in numbers from 0 to 9 and letters from A to Z
 * which are represented by numbers from 10 to 35 and outputs the
 * corresponding morse code sequence where a dot lasts for 250 ms
 * and a dash lasts for 750 ms while the space between them lasts
 * for 250 ms.
 * The morse code output can be auditory through a buzzer or visual
 * through an LED.
 * The module has been built using a state machine.
 */
module MorseOutput (
	 // Declare input ports
    input 				clock, 
    input 				switch,
	 input		[5:0] morseCode,
	 // Declare output port
    output reg 		morseOutput		
); 
// Declare registers
reg [3:0] state;
reg [27:0] counter;
// Declare local parameters
localparam N0 = 6'd0;
localparam N1 = 6'd1;
localparam N2 = 6'd2;
localparam N3 = 6'd3;
localparam N4 = 6'd4;
localparam N5 = 6'd5;
localparam N6 = 6'd6;
localparam N7 = 6'd7;
localparam N8 = 6'd8;
localparam N9 = 6'd9;
localparam A = 6'd10;
localparam B = 6'd11;
localparam C = 6'd12;
localparam D = 6'd13;
localparam E = 6'd14;
localparam F = 6'd15;
localparam G = 6'd16;
localparam H = 6'd17;
localparam I = 6'd18;
localparam J = 6'd19;
localparam K = 6'd20;
localparam L = 6'd21;
localparam M = 6'd22;
localparam N = 6'd23;
localparam O = 6'd24;
localparam P = 6'd25;
localparam Q = 6'd26;
localparam R = 6'd27;
localparam S = 6'd28;
localparam T = 6'd29;
localparam U = 6'd30;
localparam V = 6'd31;
localparam W = 6'd32;
localparam X = 6'd33;
localparam Y = 6'd34;
localparam Z = 6'd35;
// Declare local parameters for counter
localparam DOT   =  12499999; // 250 ms
localparam DASH  =  37499999; // 750 ms
localparam SPACE =  12499999; // 250 ms
localparam END   = 249999999; // 5 s
// State definitions
localparam START_STATE  = 4'b0000;
localparam DOT1_STATE   = 4'b0001;
localparam DASH1_STATE  = 4'b0010;
localparam SPACE1_STATE = 4'b0011;
localparam DOT2_STATE   = 4'b0100;
localparam DASH2_STATE  = 4'b0101;
localparam SPACE2_STATE = 4'b0110;
localparam DOT3_STATE   = 4'b0111;
localparam DASH3_STATE  = 4'b1000;
localparam SPACE3_STATE = 4'b1001;
localparam DOT4_STATE   = 4'b1010;
localparam DASH4_STATE  = 4'b1011;
localparam SPACE4_STATE = 4'b1100;
localparam DOT5_STATE   = 4'b1101;
localparam DASH5_STATE  = 4'b1110;
localparam END_STATE    = 4'b1111;
// State machine
always @(posedge clock or negedge switch) begin
	if (!switch) begin
		state <= START_STATE;
		counter <= 1'b0;
		morseOutput = 1'b0;
	end else begin
		case (state)
			START_STATE: begin // Define Start state behaviour
				morseOutput = 1'b0;
				counter <= 1'b0;
				if (morseCode == N1 || morseCode == N2 || 
					 morseCode == N3 || morseCode == N4 || 
					 morseCode == N5 || morseCode == A  || 
					 morseCode == E  || morseCode == F  || 
					 morseCode == H  || morseCode == I  || 
					 morseCode == J  || morseCode == L  || 
					 morseCode == P  || morseCode == R  || 
					 morseCode == S  || morseCode == U  || 
					 morseCode == V  || morseCode == W) begin
					state <= DOT1_STATE;
				end 
				else if (morseCode == N6 || morseCode == N7 || 
							morseCode == N8 || morseCode == N9 || 
							morseCode == N0 || morseCode == B  || 
							morseCode == C  || morseCode == D  || 
							morseCode == G  || morseCode == K  || 
							morseCode == M  || morseCode == N  || 
							morseCode == O  || morseCode == Q  || 
							morseCode == T  || morseCode == X  || 
							morseCode == Y  || morseCode == Z) begin
					state <= DASH1_STATE;
				end
			end
			DOT1_STATE: begin // Define 1st dot state behaviour
				morseOutput = 1'b1;
				if (counter == DOT) begin
					counter <= 1'b0;
					state <= SPACE1_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DOT1_STATE;
				end
			end
         DASH1_STATE: begin // Define 1st dash state behaviour
				morseOutput = 1'b1;
            if (counter == DASH) begin
					counter <= 1'b0;
					state <= SPACE1_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DASH1_STATE;
				end
			end
			SPACE1_STATE: begin // Define 1st space state behaviour
				morseOutput = 1'b0;
            if (counter == SPACE) begin
					counter <= 1'b0;
					if (morseCode == N2 || morseCode == N3 || 
						 morseCode == N4 || morseCode == N5 || 
						 morseCode == N6 || morseCode == B  || 
						 morseCode == C  || morseCode == D  || 
						 morseCode == F  || morseCode == H  || 
						 morseCode == I  || morseCode == K  || 
						 morseCode == N  || morseCode == S  || 
						 morseCode == U  || morseCode == V  || 
						 morseCode == X  || morseCode == Y) begin
						state <= DOT2_STATE;
					end 
					else if (morseCode == N1 || morseCode == N7 || 
								morseCode == N8 || morseCode == N9 || 
								morseCode == N0 || morseCode == A  || 
								morseCode == G  || morseCode == J  || 
								morseCode == L  || morseCode == M  || 
								morseCode == O  || morseCode == P  || 
								morseCode == Q  || morseCode == R  || 
								morseCode == W  || morseCode == Z) begin
						state <= DASH2_STATE;
					end
					else if (morseCode == E || morseCode == T) begin
						state <= END_STATE;
					end
				end 
				else begin
					counter <= counter + 1'b1;
					state <= SPACE1_STATE;
				end
			end
			DOT2_STATE: begin // Define 2nd dot state behaviour
				morseOutput = 1'b1;
            if (counter == DOT) begin
					counter <= 1'b0;
					state <= SPACE2_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DOT2_STATE;
				end
			end
         DASH2_STATE: begin // Define 2nd dash state behaviour
				morseOutput = 1'b1;
            if (counter == DASH) begin
					counter <= 1'b0;
					state <= SPACE2_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DASH2_STATE;
				end
			end
			SPACE2_STATE: begin // Define 2nd space state behaviour
				morseOutput = 1'b0;
            if (counter == SPACE) begin
					counter <= 1'b0;
					if (morseCode == N3 || morseCode == N4 || 
						 morseCode == N5 || morseCode == N6 || 
						 morseCode == N7 || morseCode == B  || 
						 morseCode == D  || morseCode == G  || 
						 morseCode == H  || morseCode == L  || 
						 morseCode == Q  || morseCode == R  || 
						 morseCode == S  || morseCode == V  || 
						 morseCode == X  || morseCode == Z) begin
						state <= DOT3_STATE;
					end 
					else if (morseCode == N1 || morseCode == N2 || 
								morseCode == N8 || morseCode == N9 || 
								morseCode == N0 || morseCode == C  || 
								morseCode == F  || morseCode == J  || 
								morseCode == K  || morseCode == O  || 
								morseCode == P  || morseCode == U  || 
								morseCode == W  || morseCode == Y) begin
						state <= DASH3_STATE;
					end 
					else if (morseCode == A || morseCode == I || 
								morseCode == M || morseCode == N) begin
						state <= END_STATE;
					end
				end 
				else begin
					counter <= counter + 1'b1;
					state <= SPACE2_STATE;
				end
			end
			DOT3_STATE: begin // Define 3rd dot state behaviour
				morseOutput = 1'b1;
            if (counter == DOT) begin
					counter <= 1'b0;
					state <= SPACE3_STATE;
				end else begin
					counter <= counter + 1'b1;
					state <= DOT3_STATE;
				end
			end
         DASH3_STATE: begin // Define 3rd dash state behaviour
				morseOutput = 1'b1;
            if (counter == DASH) begin
					counter <= 1'b0;
					state <= SPACE3_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DASH3_STATE;
				end
			end
			SPACE3_STATE: begin // Define 3rd space state behaviour
				morseOutput = 1'b0;
            if (counter == SPACE) begin
					counter <= 1'b0;
					if (morseCode == N4 || morseCode == N5 || 
						 morseCode == N6 || morseCode == N7 || 
						 morseCode == N8 || morseCode == B  || 
						 morseCode == C  || morseCode == F  || 
						 morseCode == H  || morseCode == L  || 
						 morseCode == P  || morseCode == Z) begin
						state <= DOT4_STATE;
					end 
					else if (morseCode == N1 || morseCode == N2 || 
								morseCode == N3 || morseCode == N9 || 
								morseCode == N0 || morseCode == J  || 
								morseCode == Q  || morseCode == V  || 
								morseCode == X  || morseCode == Y) begin
						state <= DASH4_STATE;
					end
					else if (morseCode == D || morseCode == G || 
								morseCode == K || morseCode == O || 
								morseCode == R || morseCode == S || 
								morseCode == U || morseCode == W) begin
						state <= END_STATE;
					end
				end 
				else begin
					counter <= counter + 1'b1;
					state <= SPACE3_STATE;
				end
			end
			DOT4_STATE: begin // Define 4th dot state behaviour
				morseOutput = 1'b1;
            if (counter == DOT) begin
					counter <= 1'b0;
					state <= SPACE4_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DOT4_STATE;
				end
			end
			DASH4_STATE: begin // Define 4th dash state behaviour
				morseOutput = 1'b1;
            if (counter == DASH) begin
					counter <= 1'b0;
					state <= SPACE4_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DASH4_STATE;
				end
			end
			SPACE4_STATE: begin // Define 4th space state behaviour
				morseOutput = 1'b0;
            if (counter == SPACE) begin
					counter <= 1'b0;
					if (morseCode == N5 || morseCode == N6 || 
						 morseCode == N7 || morseCode == N8 || 
						 morseCode == N9) begin
						state <= DOT5_STATE;
					end 
					else if (morseCode == N1 || morseCode == N2 || 
								morseCode == N3 || morseCode == N4 || 
								morseCode == N0) begin
						state <= DASH5_STATE;
					end
					else if (morseCode == B || morseCode == C || 
								morseCode == F || morseCode == H || 
								morseCode == J || morseCode == L || 
								morseCode == P || morseCode == Q || 
								morseCode == V || morseCode == X || 
								morseCode == Y || morseCode == Z) begin
						state <= END_STATE;
					end
				end 
				else begin
					counter <= counter + 1'b1;
					state <= SPACE4_STATE;
				end
			end
			DOT5_STATE: begin // Define 5th dot state behaviour
				morseOutput = 1'b1;
            if (counter == DOT) begin
					counter <= 1'b0;
					state <= END_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DOT5_STATE;
				end
			end
			DASH5_STATE: begin // Define 5th dash state behaviour
				morseOutput = 1'b1;
            if (counter == DASH) begin
					counter <= 1'b0;
					state <= END_STATE;
				end 
				else begin
					counter <= counter + 1'b1;
					state <= DASH5_STATE;
				end
			end
			END_STATE: begin // Define End state behaviour
				morseOutput = 1'b0;
            if (counter == END) begin
					counter <= 1'b0;
					state <= START_STATE;
				end else begin
					counter <= counter + 1'b1;
					state <= END_STATE;
				end
			end
			default: begin
				state <= START_STATE;
			end
		endcase
	end
end
endmodule

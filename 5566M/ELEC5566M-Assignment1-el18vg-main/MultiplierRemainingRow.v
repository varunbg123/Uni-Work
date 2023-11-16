/*
 * Multiplier 2 x 3 - RemainingRow
 * ----------------------------
 * By: Varun Gonsalves
 * For: University of Leeds
 * Date: 20/02/2023
 *
 * Description
 * ------------
 * This module is for the Multiplier 2 x 3. It is responsible for
 * Carrying out the operations on the second remaining row. 
 *
 */

module MultiplierRemainingRow (
    input  pp,
    input  m,
    input  q,
    input  cIn,
    
    output qOut,
    output cOut,
    output mOut,
    output s
);

//Wire used to connect the module to the Full Adder
wire A;

//Assigning QOut
assign qOut = q;

and(A,m,q);

//Full Adder added to the system
Adder1Bit Adder1Bit(
	.a(A),
	.b(pp),
	.cin(cIn),
	.cout(cOut),
	.sum(s)	
);


endmodule

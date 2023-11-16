/*
 * Multiplier 2 x 3 - FirstRow
 * ----------------------------
 * By: Varun Gonsalves
 * For: University of Leeds
 * Date: 20/02/2023
 *
 * Description
 * ------------
 * This module is for the Multiplier 2 x 3. It is responsible for
 * Carrying out the operations on the first row. 
 *
 */
 
module MultiplierFirstRow (
    input  [1:0] m,
    input  [1:0] q,
    input        cIn,
    
    output [1:0] qOut,
    output       cOut,
    output       mOut,
    output       s
);

//Wires create to carry the ouput from the AND Gates that can be fed into the 1 bit adder
wire A;
wire B;

//Assign Mout and Qout
assign mOut = m[0];

assign qOut = q;

//Connecting up the Adder1bit connections from this module. This will 
// give back the Sum
Adder1Bit Adder1Bit(
	.a(A),
	.b(B),
	.cin(cIn),
	.cout(cOut),
	.sum(s)	

);

//AND Gates that are part of the design 
and(A, q[1], m[0]);
and(B, q[0], m[1]);

endmodule

/*
 * Multiplier 2 x 3
 * ----------------------------
 * By: Varun Gonsalves
 * For: University of Leeds
 * Date: 20/02/2023
 *
 * Description
 * ------------
 * This is the top module for the Multiplier 2 x  3. It calls the files
 * FirstRow, RemainingRow and Adder1Bit. 
 *
 */


module Multiplier2x3 (
    input  [1:0] m,
    input  [2:0] q,
    
    output [4:0] p
);


//Connections between the blocks
wire [2:0] cOut;
wire sum;
wire [1:0] cIn = 0;
wire [1:0] m_out;
wire [3:0] q_out_first;
wire [1:0] q_out_second;


//First block on the top row	 
MultiplierFirstRow MultiplierFirstRow0(
		.m(m),
		.q(q[1:0]),
		.cIn(cIn[0]),
		.qOut(q_out_first[1:0]),
		.cOut(cOut),
		.mOut(m_out[0]),
		.s(p[1])
);

//Second Block on the top row
MultiplierFirstRow MultiplierFirstRow1(
		.m({1'b0,m[1]}),
		.q(q_out_first[1:0]),
		.cIn(cOut),
		.qOut(q_out_first[3:2]),
		.cOut(cOut[1]),
		.mOut(m_out[1]),
		.s(sum)
);

//First Bottom Row Block
MultiplierRemainingRow MultiplierRemainingRow0(
		.pp(sum),
		.m(m_out[0]),
		.q(q[2]),
		.cIn(cIn[1]),
		.qOut(q_out_second[0]),
		.cOut(cOut[2]),
		.s(p[2])
);

//Second Bottom Row Block
MultiplierRemainingRow MultiplierRemainingRow1(
		.pp(cOut[1]),
		.m(m_out[1]),
		.q(q_out_second[0]),
		.cIn(cOut[2]),
		.qOut(q_out_second[1]),
		.cOut(p[4]),
		.s(p[3])
);

//Adding the AND Gate for the first bit of the output
and(p[0],m[0],q[0]);



endmodule


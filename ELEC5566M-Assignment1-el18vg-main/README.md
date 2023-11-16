# 2-bit by 3-bit Multiplier Assessment

This repository contains files that carries out a 2 by 3 bit Multiplier.

The following files are Available:

| File | Purpose |
| ---  | --- |
| `Multiplier2x3.v`         | Main Module for 2bit x 3bit Multiplier |
| `MultiplerFirstRow.v`     | Submodule which comprises first row of multiplier |
| `MultiplerRemainingRow.v` | Submodule which comprises remaining rows of multiplier |
| `Multiplier2x3.sdc`       | Generic timing constraints file |
| `Structure/*`             | Diagrams of the multiplier structure |

## System Function

The function of this system is to simulate a 2 by 3 bit Multiplier. This system uses gate-level operations that take in 2 inputs a 2 bit value and a 3 bit value. The output is a 5 bit value. 

This also includes a Testbench that does exhaustive testing of all inputs and checks the outputs to make sure they are correct. 

The system is split into modules. There are 2 rows, FirstRow and the RemainingRow. The FirstRow and RemainingRow both contain 2 blocks. They are described in further detail below. 

Finally, there is testing results to show the system working as intended.

### Full Structure

![Full Structure](/Structure/fullstructure.jpg "Full Structure")

Above is the full structure of the system. How each Row is connected and how each block is connected. 

### First Row 

![First Row](/Structure/firstblockrow.jpg "First Row")

The image above shows the row which was covered before has 2 blocks. These blocks are the same inputs and output. In Verilog, this can be written as one module and be used twice in the top module. Below is this module diagram.

![First Row Block](/Structure/firstrow.jpg "First Row Block")

The image above shows the module. This has 5 inputs and 5 outputs. In the structure image, the outputs can be seen where they connect to and how it links into the RemainingRow and also into the output. Some of the outputs are also passed along the row into the next block.

### Remaining Row

![Remaining Row](/Structure/remainingblockrow.jpg "Remaining Row")

The image above shows the RemainingRow, this too has 2 blocks that are the same. These blocks are used twice in the top module code. Below is the module diagram.

![Remaining Row Block](/Structure/remainingrow.jpg "Remaining Row Block")

The image above covers the module. This has 4 inputs and 4 outputs. Looking at the overall structure there is a clear view of where this row fits in. Most importantly there are a lot of outputs from this row.

### Outputs

The 5 bit output is shared between the blocks. From the overall structure, the LSB of the output is from an AND gate. P1 is the sum out from the first row, the first block from the 1 bit adder. P2 is the sum of the RemainingRow, the first block. P3 is the sum from the remaining row, the second block. Finally, P4 is from the cout from the second block in the RemainingRow.

### 1 Bit Adder

This is an IP block used by Thomas Carpenter. This adds another complex step to this project. This takes in 3 inputs and has 2 outputs. 

### Testing Results

This is the results of all the test cases possible for the 2 by 3 Bit Multiplier.

| Multiplication | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
| :-----:  | --- | --- | --- | --- | --- | --- | --- | --- |
|0|0|0|0|0|0|0|0|0|
|1|0|1|2|3|4|5|6|7|
|2|0|2|4|6|8|10|12|14|
|3|0|3|6|9|12|15|18|21|

### Future Projects

Multipliers are the building blocks for DSP they are used widely. Also, there is a chance to parameterise this use case. Instead of a 2 by 3 Bit Multiplier, it can be N by M bit Multiplier. This way it can be changed and parameterised as needed.

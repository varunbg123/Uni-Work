# ELEC5566M Mini Project Group 1 - Morse Code Decoder

*Finn Beckitt-Marshall, Aminul Hoque and Varun Gonsalves*

## Project Description
The aim of this project is to take a Morse code input from the user and decode it into its Morse representation on the 7-segment displays as well as its character representation on the LT24 LCD display. There are many projected uses for such a project, such as learning and practicing Morse code, which is still commonly used, for example on amateur radio, as well as decoding a Morse code message received into a human-readable form.

## Git Branches
Please see [Documentation/GitBranches.md](Documentation/GitBranches.md) for a description of the Git branches.

## Inputs and Outputs

### Inputs
| Switch/Button | Function                                                |
|---------------|---------------------------------------------------------|
| SW0-5         | Morse code entry for Morse code playback function       |
| SW7           | Play Morse Code                                         |
| SW8           | Global reset switch                                     |
| SW9           | LCD character size switch - on = 2x size, off = 1x size |
| KEY3          | Dot (.)                                                 |
| KEY2          | Dash (-)                                                |
| KEY1          | Send Morse code                                         |
| KEY0          | Clear screen                                            |

### Outputs
| Output            | Function                                                                                                        |
|-------------------|-----------------------------------------------------------------------------------------------------------------|
| LCD               | Displays the recently sent characters                                                                           |
| 7-segment display | First segment displays count of number of characters received, and other segments display Morse representation. |
| LED0-4            | Display count of Morse characters entered                                                                       |
| LED6              | Displays dot or dash                                                                                            |
| LED9              | Displays the Morse code representation set using the switches                                                   |
| Buzzer            | Plays the Morse code representation set using the switches                                                      |

## Repository Files

| File Path                                | Written by                   | Description                                                                                                                                                               |
|------------------------------------------|------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| CharGen_Python/chardata.mif              | Finn (generated from script) | MIF file for instantiating the character ROM.                                                                                                                             |
| CharGen_Python/chargen.py                | Finn                         | Python script for generating the character 64-bit values.                                                                                                                 |
| CharGen_Python/pixeldroidMenuRegular.ttf | pixeldroid [4]               | Font to display on the LCD.                                                                                                                                               |
| simulation/CharacterClock_tb.v           | Finn                         | Testbench file for the CharacterClock module.                                                                                                                             |
| simulation/CharacterDraw_tb.v            | Finn                         | Testbench file for the CharacterDraw module.                                                                                                                              |
| simulation/CharacterSequential_tb.v      | Finn                         | Testbench file for the CharacterSequential module.                                                                                                                        |
| simulation/LT24FunctionalModel.v         | Thomas Carpenter [5]         | LT24 display functional model for simulation.                                                                                                                             |
| simulation/MorseDisplay_tb.v             | Aminul                       | Testbench file for the MorseDisplay module.                                                                                                                               |
| simulation/MorseLED_tb.v                 | Aminul                       | Testbench file for the MorseLED module.                                                                                                                                   |
| simulation/MorseOutput_tb.v              | Aminul                       | Testbench file for the MorseOutput module.                                                                                                                                |
| simulation/MorseToASCII_tb.v             | Finn                         | Testbench file for the MorseToASCII module.                                                                                                                               |
| CharacterClock.v                         | Finn                         | Converts ASCII code into ROM address and outputs the clocked 64-bit data from the ROM.                                                                                    |
| CharacterDraw.v                          | Finn                         | Performs the low-level draw routines of drawing the pixels on the LCD, taking in the 64-bit pixel data as the input. Uses the LT24Display module by Thomas Carpenter [1]. |
| CharacterROM_bb.v                        | Finn (autogenerated)         | Altera MegaFunction generated file                                                                                                                                        |
| CharacterROM.bsf                         | Finn (autogenerated)         | Altera MegaFunction generated file                                                                                                                                        |
| CharacterROM.qip                         | Finn (autogenerated)         | Altera MegaFunction generated file                                                                                                                                        |
| CharacterROM.v                           | Finn (autogenerated)         | Verilog instantiation function for the character ROM, using chardata.hex as the ROM file.                                                                                 |
| CharacterSequential.v                    | Finn                         | Top level charater drawing module - allows characters to be drawn sequentiallty on the LCD display                                                                        |
| chardata.hex                             | Finn                         | Hex file containing the character data to store in the ROM                                                                                                                |
| ClockDivider.v                           | Varun                        | Module that can be used to set a divided clock to the desired frequency                                                                                                                                                                          |
| DataConverter.v                          | Varun                             | Convert the raw input values to post data values to be used by other modules                                                                                                                                                                          |
| DisplayTwoDigitDecimal.v                 | Varun                             | Display two digit decimal values on two seven segment displays                                                                                                                                                                          |
| HexTo7Segment.v                          | Varun                             | Convert values from 0-F into Hex values for the seven segment display                                                                                                                                                                           |
| LT24Display.sdc                          | Thomas Carpenter [2]         | Timing constraints file for the LT24 LCD                                                                                                                                  |
| LT24Display.v                            | Thomas Carpenter [1]         | Top level LT24 display IP core                                                                                                                                            |
| MorseDecoder.qpf                         | All                          | Quartus project file                                                                                                                                                      |
| MorseDecoder.qsf                         | All                          | Quartus project file                                                                                                                                                      |
| MorseDecoderwithStateMachine.v           | Varun                             | Module that is takes in button presses and converts and sends data to other modules                                                                                                                                                                          |
| MorseDisplay.v                           | Aminul                       | Displays the Morse code on the 7-segment Displays of the DE1-SoC Board.                                                                                                   |
| MorseLED.v                               | Aminul                       | Turns on the LED for 250 ms if the DOT push button is pressed and if the DASH push button is pressed then it turns on the LED for 750 ms.                                 |
| MorseOutput.v                            | Aminul                       | Outputs the Morse code sequence for numbers from 0 to 9 and letters from A to Z, the output can be auditory through a buzzer and visual through an LED.                   |
| MorseToASCII.v                           | Finn                         | Converts the raw Morse data into ASCII codes ready for these to be displayed on the LCD.                                                                                  |
| set_LCD_pin_locs.tcl                     | Thomas Carpenter [3]         | Tcl script for setting the pin locations of the LCD display.                                                                                                              |

## References
[1] T. Carpenter, ‘LT24Display.v’. University of Leeds, Mar. 13, 2017. Accessed: May 02, 2023. [Online]. Available: https://github.com/leeds-embedded-systems

[2] T. Carpenter, ‘LT24Display.v’. University of Leeds, 2017. Accessed: May 02, 2023. [Online]. Available: https://github.com/leeds-embedded-systems

[3] T. Carpenter, ‘set_LCD_pin_locs.tcl’. University of Leeds, 2017. Accessed: May 02, 2023. [Online]. Available: https://github.com/leeds-embedded-systems

[4] pixeldroid, ‘pixeldroid fonts’. Feb. 11, 2023. Accessed: Mar. 28, 2023. [Online]. Available: https://github.com/pixeldroid/fonts

[5] T. Carpenter, ‘LT24FunctionalModel.v’. University of Leeds, Dec. 29, 2017. Accessed: May 02, 2023. [Online]. Available: https://github.com/leeds-embedded-systems


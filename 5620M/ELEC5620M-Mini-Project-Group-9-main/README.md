# ELEC5620M Mini-Project Repository: MathClub

This repository contains 2 projects `GTDrivers` and `MathClub`. Details of each project are provided in later sections.

GTDrivers contains all the hardware drivers used to interface with the peripherals onboard the DE1SoC.

MathClub uses GTDrivers along with higher-level modules to implement a mathematics-based "Who wants to be a millionare" type game.

## Gameplay
The game starts with a menu screen on the LCD displaying the high score, current difficulty mode, current volume level, and a message on the seven segment displays. The seven segment displays "Start".

![Picture of main menu](https://i.imgur.com/KgHTk9C.jpg)

Users can use button 0 to begin playing the game, buttons 1 and 2 to adjust the volume level, and switch 0 to switch between easy and hard game modes. 

![picture of board with buttons and switch labelled](https://i.imgur.com/MLMBcVE.png)

During gameplay, users are presented with a question and four options, and they must choose the correct option within the time limit using buttons 0-3. A progress bar on the LCD and the ticking of the servo represents the time remaining. The score is displayed on the seven segment display and the LEDs show progress.

![game level](https://i.imgur.com/4UJuPde.jpg)

Failure to answer the question correctly or within the time limit results in a "Game Over" message displayed on the LCD, "try again" on the seven segment displays, and an animation using the LEDs with a corresponding sound effect. 

![Game Over](https://i.imgur.com/4bTCGWm.jpg)

Correct answers trigger "Level Up!" on the LCD, "correct ans" on the seven segment displays, and another set of questions, along with an LED animation and sound effect. 

![Level Up](https://i.imgur.com/8liJzYM.jpg)

As users progress through the rounds, the LEDs display the number of levels completed. 

![Score](https://i.imgur.com/2W2NNiz.jpeg)

After answering ten questions correctly, a "Victory!" message displays on the LCD, along with an LED animation and sound effect, and the high score is updated. 

![Victory](https://i.imgur.com/FWQ9Tmj.jpg)

Users can pause the game using switch 9 and exit, adjust the volume, or return to the game using the appropriate buttons.

![Pause menu](https://i.imgur.com/cBfyZU7.jpg)



---
---

# Direct Links:

* [GTDrivers Project](#gtdrivers)
    1. [Audio Driver](#audio-driver-usage)
    2. [LCD Driver](#lcd-driver-usage)
    3. [LED Driver](#led-driver-usage)
    4. [SDCard Driver](#sdcard-driver-usage)
    5. [Servo Drive](#de1soc_servo-driver-usage)
    6. [SevenSeg Driver](#sevenseg-driver-usage)
    7. [Timer Driver](#timer-driver-usage)

* [MathClub Project](#mathclub-project)
    1. [GameEngine Module](#game-engine-module-usage)
    2. [GraphicsEngine Module](#graphics-engine-module-usage)
    3. [QuestionGenerator Module](#question-generator-module-usage)


---
---

## GTDrivers
The following files are provided in the GTDrivers project:

| File | Purpose |
| ---- | ------- |
| `.project`      | DS-5 project definition, Configured for DE1-SoC |
| `.cproject`     | As above |
| `Audio/AudioOutput.h`  | The header file which defines the interface for the audio driver.|
| `Audio/AudioOutput.c`  | The implementation file for the audio driver.|
| `BasicFont/BasicFont.h`  | The header file for BasicFont.c|
| `BasicFont/BasicFont.c`  | Contains 8x5 bitmaps for all characters. |
| `DE1SoC_WM8731/DE1SoC_WM8731.h`  | The header file which defines the interface for the the WM8731 audio peripheral driver.|
| `DE1SoC_WM8731/DE1SoC_WM8731.c`  | The implementation file for the WM8731 audio peripheral driver.|
| `FatFS/*`  | All files pertaining to the FatFS file system library.|
| `HPS_I2C/HPS_I2C.h`  | The header file which defines the interface for the HPS I2C controller driver.|
| `HPS_I2C/HPS_I2C.c`  | The implementation file for the HPS I2C controller driver|
| `HPS_usleep/HPS_usleep.h`  | The header file which defines the interface for a HPS SP1 Timer Based "usleep"|
| `HPS_usleep/HPS_usleep.c`  | The implementation file for the HPS SP1 Timer Based "usleep"|
| `HPS_Watchdog/HPS_Watchdog.h`  | The header file which defines the interface for the HPS Watchdog|
| `LCD/LCD.h`  | The header file which defines the interface for the LCD driver.|
| `LCD/LCD.c`  | The implementation file for the LCD driver.|
| `LED/LED.h`  | The header file which defines the interface for the LED driver.|
| `LED/LED.c`  | The implementation file for the LED driver.|
| `SDCard/SDCard.h`  | The header file which defines the interface for the SDCard driver.|
| `SDCard/SDCard.c`  | The implementation file for the SDCard driver.|
| `Servo/DE1SoC_Servo.h`  | The header file which defines the interface for the Servo driver.|
| `Servo/DE1SoC_Servo.c`  | The implementation file for the Servo driver.|
| `SevenSeg/SevenSeg.h`  | The header file which defines the interface for the seven segment display driver.|
| `SevenSeg/SevenSeg.c`  | The implementation file for the seven segment display driver.|
| `Timer/Timer.h`  | The header file which defines the interface for the HPS Timer driver.|
| `Timer/Timer.c`  | The implementation file for the HPS Timer driver.|

Please note the project will need configuring in DS-5 before it will compile.

---
## Audio Driver Usage
---
This driver exposes 2 functions:

## `AUDIOOUTPUT_playTone`
Function that takes in a certain frequency and processes it to fulfill a single iteration of generating the desired output to be sent to the desired channel(s)

NOTE: Ensure that the function is within a loop so it can generate the whole waveform to be heard
### Arguments
The signature for the function is given below:

```c
int AUDIOOUTPUT_playTone(double frequency, double volume, unsigned int channel)
```

From the signature it can be seen that the function takes 3 arguments.

`frequency`:              Frequency to be played (continuous positive decimal values)

`volume`:                 Volume of the sample to be sent to the channel (0 - 100)

`channel`:                Channel the desired sample is to be played in
                        0 - Both Channels, 1 - Left Channel, 2 - Right Channel

### Example Usage
```c
AUDIOOUTPUT_playTone(C4, 50.0, AUDIO_BOTHCHANNELS);
```

---
---

## `AUDIOOUTPUT_writeToChannel`
This Function writes a sample value to the desired channl(s)
### Arguments
The signature for the function is given below:

```c
void AUDIOOUTPUT_writeToChannel(unsigned int channel_choice, signed int left_vlaue, signed int right_value)
```

From the signature it can be seen that the function takes 3 arguments.

`channel_choice`:        The desired FIFO to write to and consequently the desired Channel to ouput on (0 - 3)

`left_value`:            Value to write to the left FIFO pointer (continuous positive decimal values)

`right_value`:           Value to write to the right FIFO pointer (continuous positive decimal values)

### Example Usage
```c
AUDIOOUTPUT_writeToChannel(AUDIO_BOTHCHANNELS, 124124, 3252352);
```

---
## LCD Driver Usage
---
This driver exposes 17 functions out of which 9 are new:

## `LCD_update`
Update the screen contents.
All the LCD_draw* methods update a digital representation
of the screen. In order, to see the drawings on the screen
this method needs to be called.
Every time this method is the called, the contents
of the digital representation are written to the LCD.
### Example Usage
```c
LCD_update();
```

---
---

## `LCD_drawPixel`
Draw a pixel.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawPixel(unsigned int x, unsigned int y, unsigned short colour)
```

From the signature it can be seen that the function takes 3 arguments.

`x, y`:        x,y coordinates of the pixel

`color`:       color of the pixel

### Example Usage
```c
LCD_drawPixel(1,1, 0x0000);
```

---
---

## `LCD_drawLine`
Draw a line between two specified points with specified color
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawLine(int x1, int y1, int x2, int y2, unsigned short color)
```

From the signature it can be seen that the function takes 5 arguments.

`x1, y1`:       x,y coordinates of first point

`x2, y2`:       x,y coordinates of second point

`color`:        color of the line


### Example Usage
```c
LCD_drawLine(0, 0, 45, 50,  0x0000);
```
---
---

## `LCD_setColor`
Set the whole screen to a color
### Arguments
The signature for the function is given below:

```c
signed int LCD_setColor(unsigned int R, unsigned int G, unsigned int B)
```

From the signature it can be seen that the function takes 3 arguments.

`R`: 0-255 value for Red component

`G`: 0-255 value for Green component

`B`: 0-255 value for Blue component

### Example Usage
```c
LCD_setColor(255, 0, 255);
```
---
---

## `LCD_drawTriangle`
Draw a triangle with specified vertices and fill.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned short color, bool fill);
```

From the signature it can be seen that the function takes 8 arguments.

`x1, y1`:     x,y coordinates of first vertice
`x2, y2`:     x,y coordinates of second vertice
`x3, y3`:     x,y coordinates of second vertice
`color`:      color of the border/fill
`fill`:       false - for only border, true - for fill with color

### Example Usage
```c
LCD_drawTriangle(0, 0, 50, 50, 100, 10, 0x0000, true);
```
---
---

## `LCD_drawRectangle`
Draw a rectangle with specified origin, height, width and fill.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawRectangle(int x, int y, int height, int width, unsigned short color, bool fill)
```

From the signature it can be seen that the function takes 6 arguments.

`x, y`:       x,y coordinates of top-left corner
`height`:     height of the rectangle in pixels
`width`:      width of the rectangle in pixels
`color`:      color of the border/fill
`fill`:       false - for only border, true - for fill with color

### Example Usage
```c
LCD_drawRectangle(0, 0, 50, 50, 0x0000, true);
```
---
---

## `LCD_drawCircle`
Draw a rectangle with specified origin, height, width and fill.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawCircle(int x, int y, int radius, unsigned short color, bool fill)
```

From the signature it can be seen that the function takes 5 arguments.

`x, y`:        x,y coordinates of the center
`raidus`:      radius of the circle in pixels
`color`:       color of the border/fill
`fill`:        false - for only border, true - for fill with color

### Example Usage
```c
LCD_drawCircle(100, 100, 50, 0x0000, true);
```
---
---

## `LCD_drawChar`
Draw a character on the screen.
e.g. alphabet, number, alphanumeric etc.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawChar(char character, int x, int y, unsigned short color, int size)
```

From the signature it can be seen that the function takes 5 arguments.

`character`:    the character to be drawn

`x, y`:         x,y coordinates of bottom-left corner

`color`:        color of the character

### Example Usage
```c
LCD_drawChar('A', 100, 50, 0x0000, 3);
```
---
---

## `LCD_drawText`
Draw a string of text on the screen.
### Arguments
The signature for the function is given below:

```c
signed int LCD_drawText(char text[], int x, int y, unsigned short color, int size);
```

From the signature it can be seen that the function takes 5 arguments.

`text`:         the text to be drawn

`x, y`:         x,y coordinates of bottom-left corner

`color`:        color of the character

### Example Usage
```c
LCD_drawText('Hello', 100, 50, 0x0000, 3);
```
---
---
## LED Driver Usage
---
This driver exposes 2 functions:

## `LED_initialise`
Set the base address for the LED register
### Arguments
The signature for the function is given below:

```c
signed int LED_initialise( unsigned int base_address)
```

From the signature it can be seen that the function takes 1 argument.

`base_address`:              base address for the LED

### Example Usage
```c
LED_initialise(0xFF2000020);
```

---
---

## `LED_write`
This function is to write values to the LEDs pointer
### Arguments
The signature for the function is given below:

```c
signed int LED_write(unsigned int value)
```
From the signature it can be seen that the function takes 1 argument.

`Value`:         This value is written to the LED pointer
### Example Usage
```c
LED_write(0x022);
```
---
---

## `LED_setValueInRange`
This function will use all 10 LEDs to diplay a value in the 
min-max range. It converts the value to a range of 1-10. Which is then
written to the LED register
### Arguments
The signature for the function is given below:

```c
unsigned int LED_setValueInRange(float min, float max, float value);
```
From the signature it can be seen that the function takes 3 arguments.

`min`:     minimum value
`max`:     maximum value
`value`:   value in the min max range
### Example Usage
```c
LED_setValueInRange(0, 30, 15);
```
---
## SDCard Driver Usage
---
This driver exposes 6 functions:

## `SDCARD_createFile`
This function is to create a file.
### Arguments
The signature for the function is given below:

```c
void SDCARD_createFile(char filename[])
```

From the signature it can be seen that the function takes 1 argument.

`filename`:   Name of the file to create

### Example Usage
```c
SDCARD_createFile("hello.txt");
```

---
---

## `SDCARD_writeToFile`
This function will write to the file.
The contents of the file will be overwritten.
### Arguments
The signature for the function is given below:

```c
void SDCARD_writeToFile(char filename[], char text[])
```

From the signature it can be seen that the function takes 2 arguments.

`filename`:     the name of the file to be written to

`text`:         the text to be written to the file

### Example Usage
```c
SDCARD_writeToFile("hello.txt", "Hello World");
```
---
---

## `SDCARD_mount`
Function used to Mount the SD Card to initiliase the file system
### Arguments
The signature for the function is given below:

```c
void SDCARD_mount(void)
```
### Example Usage
```c
SDCARD_mount();
```
---
---

## `SDCARD_readLine`
This function will return the contents of the specified file
up to the specified length.
### Arguments
The signature for the function is given below:

```c
void SDCARD_readLine(char filename[], unsigned int length, char line[])
```
From the signature it can be seen that the function takes 2 arguments.

`filename`:     the name of the file to be read

`length`:       the length of text to be read

`line`:         Array to store the line in.

### Example Usage
```c
char line[100];
SDCARD_readLine("hello.txt", 100, line);
```
---
## `SDCARD_readFile`
Function to read the entire file
### Arguments
The signature for the function is given below:

```c
unsigned int SDCARD_readFile(char filename[], char **lines)
```
From the signature it can be seen that the function takes 2 arguments.

`filename`:		    The name of the file to read
`lines`:			2D arrays to store the lines read from file

#### Output:
`num_lines_read`:	The number of lines read from the file

### Example Usage
```c
char lines[100][100];
SDCARD_readFile("hello.txt", lines);
printf(lines[0]);
```
---
---
## `SDCARD_checkFileExists`
Function to check if a file exists
### Arguments
The signature for the function is given below:

```c
bool SDCARD_checkFileExists(char filename[])
```
From the signature it can be seen that the function takes 1 argument.

`filename`:		    The name of the file to check for

#### Output:
`boolean`:	    true if file exists else false

### Example Usage
```c
SDCARD_checkFileExists("hello.txt");
```
---

---
## DE1SoC_Servo Driver Usage
---
This driver exposes 6 functions:

## `Servo_initialise`
Function to initialise the Servo Controller
### Arguments
The signature for the function is given below:

```c
signed int Servo_initialise( unsigned int base_address )
```

From the signature it can be seen that the function takes 1 argument.

`base_address`:   Base address of the servo register

### Example Usage
```c
Servo_initialise(0x6576523);
```

---
---

## `Servo_isInitialised`
This function will Check if driver initialised
### Arguments
The signature for the function is given below:

```c
bool Servo_isInitialised( void )
```
### Example Usage
```c
Servo_isInitialised();
```
---
---

## `Servo_enable`
Enable/Disable a servo
When a servo is disabled, its control pin can be used as an input for a switch or other device.
### Arguments
The signature for the function is given below:

```c
signed int Servo_enable( unsigned int servo_id, bool set_enabled)
```
From the signature it can be seen that the function takes 2 arguments.

`servo_id`:   is the number of the servo to control
`set_enabled`: is true to enable, false to disable.
### Example Usage
```c
Servo_enable(0, true);
```
---
---
## `Servo_setPositionInRange`
This function will set the servo's position based on the specific min and max range. 
The min and the max is mapped to the maximum and minimum positions of the servo.
### Arguments
The signature for the function is given below:

```c
void Servo_setPositionInRange(int servo_id, float min, float max, float position)
```
From the signature it can be seen that the function takes 4 arguments.

`servo_id`:	index of servo to control
`min`:		minimum value
`max`:		maximum value
`position`:	position of servo

### Example Usage
```c
Servo_setPositionInRange(0, 0, 40, 20);
```
---
---
## SevenSeg Driver Usage
---
This driver exposes 7 functions:

## `SevenSeg_initialise`
Function to initialise the seven seg Controller
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_initialise( unsigned int base_address )
```

From the signature it can be seen that the function takes 1 argument.

`base_address`:   Base address of the sevenseg register

### Example Usage
```c
SevenSeg_initialise(0x6576523);
```

---
---

## `SevenSeg_isInitialised`
This function will Check if driver initialised
### Arguments
The signature for the function is given below:

```c
bool SevenSeg_isInitialised( void )
```
### Example Usage
```c
SevenSeg_isInitialised();
```
---
---

## `SevenSeg_write`
Low level set function to send a byte value to a selected display.
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_write( unsigned int servo_id, bool set_enabled)
```
From the signature it can be seen that the function takes 2 arguments.

`display`:		index of the display to update (0-5)
`value`: 			byte to set display to.
### Example Usage
```c
SevenSeg_write(0, 0x45);
```
---
---
## `SevenSeg_displayCharacter`
Display a character on a specified display.
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_displayCharacter(unsigned int display, char character)
```
From the signature it can be seen that the function takes 2 arguments.

`display`:		index of the display to update (0-5)
`character`:		the character to display

### Example Usage
```c
SevenSeg_displayCharacter(0, 'a');
```
---
---
## `SevenSeg_displayCharacter`
Display a character on a specified display.
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_displayCharacter(unsigned int display, char character)
```
From the signature it can be seen that the function takes 2 arguments.

`display`:		index of the display to update (0-5)
`character`:		the character to display

### Example Usage
```c
SevenSeg_displayCharacter(0, 'a');
```
---
---
## `SevenSeg_displayWord`
Display a word using all 6 seven segment displays from left to right
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_displayWord(unsigned int display, char character)
```
From the signature it can be seen that the function takes 1 argument.

`word`:		The word to be displayed (max length = 6)

### Example Usage
```c
SevenSeg_displayWord('abcd');
```
---
---
## `SevenSeg_displayDigit`
Display a digit on a specified display.
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_displayDigit(unsigned int display, int number)
```
From the signature it can be seen that the function takes 2 arguments.

`display`:		index of the display to update (0-5)
`number`:		the digit to display (0-9)

### Example Usage
```c
SevenSeg_displayDigit(0, 3);
```
---
---
## `SevenSeg_displayNumber`
Display a number using all 6 seven segment displays from left to right.
If the number is longer than 6 digits only the last 6 digits are displayed.
### Arguments
The signature for the function is given below:

```c
signed int SevenSeg_displayNumber(int number)
```
From the signature it can be seen that the function takes 1 argument.

`number`:		   The number to be displayed.

### Example Usage
```c
SevenSeg_displayNumber(2345);
```
---
---
## Timer Driver Usage
---
This driver exposes 13 functions:

## `Timer_initialise`
Function to initialise the timer Controller
### Arguments
The signature for the function is given below:

```c
signed int Timer_initialise( unsigned int base_address )
```

From the signature it can be seen that the function takes 1 argument.

`base_address`:   Base address of the timer register

### Example Usage
```c
Timer_initialise(0x6576523);
```

---
---

## `Timer_isInitialised`
This function will Check if driver initialised
### Arguments
The signature for the function is given below:

```c
bool Timer_isInitialised( void )
```
### Example Usage
```c
Timer_isInitialised();
```
---
---

## `Timer_setLoad`
Setting the timer load value into the register
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setLoad(unsigned int load_value)
```
From the signature it can be seen that the function takes 1 argument.

`load_value`: 	load value to set timer
### Example Usage
```c
Timer_setLoad(1342);
```
---
---
## `Timer_getValue`
Getting the current timer value
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_getValue(void)
``

### Example Usage
```c
Timer_getValue();
```
---
---
## `Timer_setPrescalar`
Setting the Prescalar Value into the control register
The prescalar will scale the load value by (input + 1).
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setPrescalar(unsigned int prescalar_value);
```
From the signature it can be seen that the function takes 1 argument.

`preScalar_value`:	input value for the prescalar

### Example Usage
```c
Timer_setPrescalar(1);
```
---
---
## `Timer_setEnable`
Setting the Enable Value into the control register.
This will enable the timer
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setEnable(unsigned int enable_value)
```
From the signature it can be seen that the function takes 1 argument.

`enable_value`:	input value for the enable

### Example Usage
```c
Timer_setEnable(1);
```
---
---
## `Timer_setAutomaticReload`
Setting the Automatic Reload Value into the control register
This will reset the value once the the timer has finished counting down
### Arguments
The signature for the function is given below:

```c
signed int Timer_setAutomaticReload(unsigned int display, int number)
```
From the signature it can be seen that the function takes 1 argument.

`automatic_reloadValue`:	input value for the automatic reload

### Example Usage
```c
Timer_setAutomaticReload(0);
```
---
---
## `Timer_setISRValue`
Setting the ISR Value into the control register
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setISRValue(unsigned int ISR_value);
```
From the signature it can be seen that the function takes 1 argument.

`ISR_value`:	input value for the ISR

### Example Usage
```c
Timer_setISRValue(1);
```
---
---
## `Timer_setControl`
Setting the whole control register in one function. This includes the :
Prescalar, ISR, Overflow and the Enable Timer
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setControl(unsigned int prescalar_value, unsigned int ISR, unsigned int overflow, unsigned int enable_timer);

```
From the signature it can be seen that the function takes 4 arguments.

`prescalar_value`:		input value for the prescalar
`ISR` :				input value for the ISR
`overflow`:	input value for the automatic reload
`enable_timer`:			input value for the enable

### Example Usage
```c
Timer_setControl(1, 1, 0, 1);
```
---
---
## `Timer_getInterruptStatus`
Getting the interrupt status
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_getInterruptStatus(void)

```

### Example Usage
```c
Timer_getInterruptStatus();
```
---
---
## `Timer_resetInterrupt`
Reseting the interrupt by setting the interrupt to 1 to clear the flag
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_resetInterrupt(void)

```

### Example Usage
```c
Timer_resetInterrupt();
```
---
---
## `Timer_setPeriod`
Setting the Timer period. The input for this will be in milliseconds.
This will set the prescalar, isr, automatic reload and the enable automatically from the input
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_setPeriod(unsigned int time);
```
From the signature it can be seen that the function takes 1 argument.

`time`:	input value for the time in ms

### Example Usage
```c
Timer_setPeriod(1000);
```
---
---
## `Timer_getValueMS`
Getting the current timer value but in milliseconds.
This can be used for multiple clock and be used in ms. However when
using mutiple clock make sure to set the update period as a multiple of the Period
### Arguments
The signature for the function is given below:

```c
unsigned int Timer_getValueMS(void)

```
Returns:
`time_ms`:	current time from the pointer in milliseconds

### Example Usage
```c
Timer_getValueMS();
```
---
---

## MathClub Project
The following files are provided in the MathClub project:

| File | Purpose |
| ---- | ------- |
| `.project`      | DS-5 project definition, Configured for DE1-SoC |
| `.cproject`     | As above |
| `GameEngine/GameEngine.h`  | The header file which defines the interface for GameEngine module.|
| `GameEngine/GameEngine.c`  | The implementation file for the GameEngine module.|
| `GraphicsEngine/GraphicsEngine.h`  | The header file for the GraphicsEngine module |
| `GraphicsEngine/GraphicsEngine.c`  | The implementation file for the GraphicsEngine module.|
| `QuestionGenerator/QuestionGenerator.h`  | The header file for the QuestionGenerator module |
| `QuestionGenerator/QuestionGenerator.c`  | The implementation file for the QuestionGenerator module.|
| `main.c`  | The implementation file for the game's state machine.|

Please note the project will need configuring in DS-5 before it will compile.

---
## Game Engine Module Usage
---
This module exposes 25 functions, the key methods are provided below:

## `GameEngine_levelUp`
Function that displays the level up animation for the game.
LCD displays Level Up!
SevenSegment displays "correct ans"
and LEDs show pattern.

### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_levelUp(void)
```
### Example Usage
```c
GameEngine_levelUp();
```

---
---
## `GameEngine_gameOver`
Function that displays the level up animation for the game.
LCD displays Game Over
SevenSegment displays "try again"
and LEDs show pattern.

### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_gameOver(void)
```
### Example Usage
```c
GameEngine_gameOver();
```

---
---
## `GameEngine_gameOver`
Function that displays the level up animation for the game.
LCD displays Game Over
SevenSegment displays "try again"
and LEDs show pattern.

### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_gameOver(void)
```
### Example Usage
```c
GameEngine_gameOver();
```

---
---
## `GameEngine_celebrate`
Function that displays the level up animation for the game.
LCD displays Victory!
SevenSegment displays "victory"
and LEDs show pattern.

### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_celebrate(void)
```
### Example Usage
```c
GameEngine_celebrate();
```

---
---
## `GameEngine_setState`
Sets the current state of the game
### Arguments
The signature for the function is given below:

```c
void GameEngine_setState(unsigned int new_state);

```
Inputs:
     `new_state`:  The state to set the game in, values can be one of:
                 `GAMEENGINE_MAINMENU`, `GAMEENGINE_PLAYING`, `GAMEENGINE_PAUSED`
                 `GAMEENGINE_LEVELUP`, `GAMEENGINE_GAMEOVER`, `GAMEENGINE_WIN`
### Example Usage
```c
GameEngine_setState(GAMEENGINE_MAINMENU);
```

---

---
## `GameEngine_getState`
Can be used to get the current state of the game.
### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_getState(void);

```
Output:
     Returns the current state of the game as an unsigned int.
### Example Usage
```c
GameEngine_getState();
```

---
---
## `GameEngine_setLevel`
Sets the current level of the game
Changing the level generates a new question at the
required difficulty as well.
### Arguments
The signature for the function is given below:

```c
void GameEngine_setLevel(unsigned int new_level);

```
Inputs:
     `new_level`:  The level to set within range 0-9
### Example Usage
```c
GameEngine_setLevel(6);
```

---
---
## `GameEngine_setGameMode`
Sets the current mode of the game i.e. easy or hard.
### Arguments
The signature for the function is given below:

```c
void GameEngine_setGameMode(unsigned int new_game_mode);

```
Inputs:
     `new_game_mode`:  The mode to set, values can be one of:
                     GAMEENGINE_EASY or GAMEENGINE_HARD
### Example Usage
```c
GameEngine_setGameMode(GAMEENGINE_EASY);
```

---
---
## `GameEngine_setScore`
Sets the current score of the game
### Arguments
The signature for the function is given below:

```c
void GameEngine_setScore(unsigned int new_score);
```
Inputs:
    `new_score`:  The score to set  within range 0-10
### Example Usage
```c
GameEngine_setScore(2);
```

---

---
## `GameEngine_setTimeLimit`
 Sets the time limit for each question of the game
### Arguments
The signature for the function is given below:

```c
void GameEngine_setTimeLimit(float new_time_limit);
```
Inputs:
     `new_time_limit`:  The time limit to set  within range 0-30
### Example Usage
```c
GameEngine_setTimeLimit(15.0);
```

---
---
## `GameEngine_setVolume`
Sets the volume level for the game within range 0-10
### Arguments
The signature for the function is given below:

```c
void GameEngine_setVolume(float new_volume);
```
Inputs:
     `new_volume`:  The volume level to set within range 0-10
### Example Usage
```c
GameEngine_setVolume(15.0);
```

---
---
## `GameEngine_initialise`
Intialises game's state to default values. This includes:
Setting the score to 0
Setting the level to 0
Setting the game mode to GAMEENGINE_EASY
Setting the state to GAMEENGINE_MAINMENU
Setting the volume to 5.0
### Arguments
The signature for the function is given below:

```c
void GameEngine_initialise(char *storage_filename);
```
Input:
     `storage_filname`:    name of the file used for storage of highscore.
### Example Usage
```c
GameEngine_initialise("store.txt");
```

---
---
## `GameEngine_enterOption`
Used to select an option for the current question.
If the correct option is selected, the level is incremented,
score is incremented, and level up screen is displayed.
If the wrong option is selected, score and level are reset,
high score is updated and game over screen is displayed.
### Arguments
The signature for the function is given below:

```c
void GameEngine_enterOption(unsigned int option);
```
Inputs:
     `option`:  The option selected by the player, values can be:
              `GAMEENGINE_OPTION1`, `GAMEENGINE_OPTION2`,
              `GAMEENGINE_OPTION3` or `GAMEENGINE_OPTION4`
### Example Usage
```c
GameEngine_enterOption(GAMEENGINE_OPTION3);
```

---
---
## `GameEngine_displayMainMenu`
Displays the main menu of the game.
### Arguments
The signature for the function is given below:

```c
void GameEngine_displayMainMenu(void);
```
### Example Usage
```c
GameEngine_displayMainMenu();
```

---
---
## `GameEngine_displayPauseMenu`
Displays the pause menu of the game.
### Arguments
The signature for the function is given below:

```c
void GameEngine_displayPauseMenu(void);
```
### Example Usage
```c
GameEngine_displayPauseMenu();
```
---
---
## `GameEngine_displayLevel`
Displays the question, options and time remaining for
the current question.
### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_displayLevel(float time_remaining);
```
Inputs:
  `time_remaining`:  the amount of time remaining out of the time limit
### Example Usage
```c
GameEngine_displayLevel(30.0);
```
---
---
## `GameEngine_reset`
Resets the game's state, level, and score.
### Arguments
The signature for the function is given below:

```c
unsigned int GameEngine_reset(void);
```
### Example Usage
```c
GameEngine_reset();
```
---

---
## Graphics Engine Module Usage
---
This module exposes 11 functions, the key methods are provided below:

## `GraphicsEngine_setBackground`
Sets the background color of the screen.
Light wrapper around LCD_setColor

### Arguments
The signature for the function is given below:

```c
void GraphicsEngine_setBackground(unsigned int R, unsigned int G, unsigned int B);
```
Inputs:
  `R`:  Red value
  `G`:  Green value
  `B`:  Blue value
### Example Usage
```c
GraphicsEngine_setBackground(0,0,0);
```

---
---
## `GraphicsEngine_drawSoundIcon`
Draws a sound/volume icon at specified x,y (bottom-left)

### Arguments
The signature for the function is given below:

```c
void GraphicsEngine_drawSoundIcon(unsigned int x, unsigned int y);

```
Inputs:
     `x, y`:        x,y coordinates of bottom-left of icon
### Example Usage
```c
GraphicsEngine_drawSoundIcon(10,10);
```

---
---
## `GraphicsEngine_drawProgressBar`
Draws a progress bar widget on the screen at specified x,y with
area proportional to `value` filled and specified color.
### Arguments
The signature for the function is given below:

```c
void GraphicsEngine_drawProgressBar(unsigned int x, unsigned int y, float height, float width, float value, unsigned int color[3]);

```
Inputs:
    `x,y`:        coordinates of bottom left of progress bar
    `height`:     height of progress bar
    `width`:      width of progress bar
    `value`:      value of progress bar out of 100
    `color`:      color of progress bar in R,G,B
### Example Usage
```c
GraphicsEngine_drawProgressBar(100,100,20,200,50,RED);
```

---
---
## `GraphicsEngine_drawLogo`
Draws the MathClub logo at specified x,y (bottom-left)
### Arguments
The signature for the function is given below:

```c
void GraphicsEngine_drawLogo(unsigned int x, unsigned int y);

```
Inputs:
    `x, y`:        x,y coordinates of bottom-left of logo
### Example Usage
```c
GraphicsEngine_drawLogo(50,50);
```
---
---
## `GraphicsEngine_update`
Updates the screen contents.
Light wrapper around LCD_update
### Arguments
The signature for the function is given below:

```c
void GraphicsEngine_update(void);

```
### Example Usage
```c
GraphicsEngine_update();
```
---

---
## Question Generator Module Usage
---
This module exposes 1 function:
---
## `QuestionGenerator_generateQuestion`
This function is generates a simple math question involving a
random operation between 2 random numbers based on the specified difficulty
The operation will be either add, subtract, multiple or divide.
The numbers are generated within the range 1-100
### Arguments
The signature for the function is given below:

```c
QuestionResult QuestionGenerator_generateQuestion(unsigned int difficulty);
```
Input:
		`difficulty`:	   This will be between 1-3, which will set the difficulty of the question

Output:
		`QuestionResult`: A struct containing the following properties
			`text`:		This will be the text of the question
			`answer`:		This is the answer to the question as in integer
			`options`:	4 options are provided including the answer
### Example Usage
```c
QuestionGenerator_generateQuestion(2);
```
---
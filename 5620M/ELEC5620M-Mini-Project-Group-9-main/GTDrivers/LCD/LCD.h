/*
 * LCD Display Controller Driver
 * ------------------------------
 * Description:
 * Driver for the LCD Display Controller
 *
 * Company: University of Leeds
 * Author: T Carpenter, Kaif Kutchwala
 * Emmanuel Leo and Varun Gonsalves
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 05/02/2017 | Creation of driver
 * 20/10/2017 | Update driver to match new styles
 * 06/05/2023 | Modify driver for Mini-Project
 */

#ifndef DE1SoC_LCD_H_
#define DE1SoC_LCD_H_

// Include required header files
#include <math.h>
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Useful Functions
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

// Error Codes
#define LCD_SUCCESS 0
#define LCD_ERRORNOINIT -1
#define LCD_INVALIDSIZE -4
#define LCD_INVALIDSHAPE -6

// Size of the LCD
#define LCD_WIDTH 240
#define LCD_HEIGHT 320

// Some basic colours
#define LCD_BLACK (0x0000)
#define LCD_WHITE (0xFFFF)
#define LCD_RED (0x1F << 11)
#define LCD_GREEN (0x1F << 6)
#define LCD_BLUE (0x1F << 0)
#define LCD_YELLOW (LCD_RED | LCD_GREEN)
#define LCD_CYAN (LCD_GREEN | LCD_BLUE)
#define LCD_MAGENTA (LCD_BLUE | LCD_RED)

// Function to initialise the LCD
//  - Returns 0 if successful
signed int LCD_initialise(unsigned int pio_base_address, unsigned int pio_hw_base_address);

// Check if driver initialised
//  - returns true if initialised
bool LCD_isInitialised(void);

// Function for writing to LCD Registers (using dedicated HW)
// You must check LCD_isInitialised() before calling this function
void LCD_write(bool isData, unsigned short value);

// Function for configuring LCD reset/power (using PIO)
// You must check LCD_isInitialised() before calling this function
void LCD_powerConfig(bool isOn);

// Function to clear display to a set colour
//  - Returns 0 if successful
signed int LCD_clearDisplay(unsigned short colour);

// Function to convert Red/Green/Blue to RGB565 encoded colour value
unsigned short LCD_makeColour(unsigned int R, unsigned int G, unsigned int B);

// Function to set the drawing window on the display
//   Returns 0 if successful
signed int LCD_setWindow(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height);

// Copy frame buffer to display
//  - returns 0 if successful
signed int LCD_copyFrameBuffer(const unsigned short *framebuffer, unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height);

/**
 * LCD_update
 *
 * Update the screen contents.
 * All the LCD_draw* methods update a digital representation
 * of the screen. In order, to see the drawings on the screen
 * this method needs to be called.
 *
 * Every time this method is the called, the contents
 * of the digital representation are written to the LCD.
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_update(void);

/**
 * LCD_drawPixel
 *
 * Draw a pixel.
 *
 * Inputs:
 *      x, y:        x,y coordinates of the pixel
 *      color:       color of the pixel
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawPixel(unsigned int x, unsigned int y, unsigned short colour);

/**
 * LCD_drawLine
 *
 * Draw a line between two specified points with specified color
 *
 * Inputs:
 *      x1, y1:       x,y coordinates of first point
 *      x2, y2:       x,y coordinates of second point
 *      color:        color of the line
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawLine(int x1, int y1, int x2, int y2, unsigned short color);

/**
 * LCD_setColor
 *
 * Set the whole screen to a color
 *
 * Inputs:
 *      R: 0-255 value for Red component
 *      G: 0-255 value for Green component
 *      B: 0-255 value for Blue component
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_setColor(unsigned int R, unsigned int G, unsigned int B);

/**
 * LCD_drawTriangle
 *
 * Draw a triangle with specified vertices and fill.
 *
 * Inputs:
 *      x1, y1:     x,y coordinates of first vertice
 *      x2, y2:     x,y coordinates of second vertice
 *      x3, y3:     x,y coordinates of second vertice
 *      color:      color of the border/fill
 *      fill:       false - for only border, true - for fill with color
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned short color, bool fill);

/**
 * LCD_drawRectangle
 *
 * Draw a rectangle with specified origin, height, width
 * and fill.
 *
 * Inputs:
 *      x, y:       x,y coordinates of top-left corner
 *      height:     height of the rectangle in pixels
 *      width:      width of the rectangle in pixels
 *      color:      color of the border/fill
 *      fill:       false - for only border, true - for fill with color
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawRectangle(int x, int y, int height, int width, unsigned short color, bool fill);

/**
 * LCD_drawCircle
 *
 * Draw a circle with specified center,radius, color
 * and fill.
 *
 * Inputs:
 *      x, y:        x,y coordinates of the center
 *      raidus:      radius of the circle in pixels
 *      color:       color of the border/fill
 *      fill:        false - for only border, true - for fill with color
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawCircle(int x, int y, int radius, unsigned short color, bool fill);

/**
 * LCD_drawChar
 *
 * Draw a character on the screen.
 * e.g. alphabet, number, alphanumeric etc.
 *
 * Inputs:
 *         character:    the character to be drawn
 *         x, y:         x,y coordinates of bottom-left corner
 *         color:        color of the character
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawChar(char character, int x, int y, unsigned short color, int size);

/**
 * LCD_drawText
 *
 * Draw a string of text on the screen.
 *
 * Inputs:
 *      text:        the text to be drawn
 *      x, y:        x,y coordinates of bottom-left corner
 *      color:       color of the text
 *      size:        the size of the text
 *
 * Output: Returns LCD_Success if Succesfully Completed
 *         Returns LCD_ERRORNOINIT if LCD is not initiliased
 */
signed int LCD_drawText(char text[], int x, int y, unsigned short color, int size);

#endif /*DE1SoC_LCD_H_*/

/*
Draw a pixel
LCD_setWindow(x,y,1,1);
LCD_write(true,LCD_makeColour(R,G,B));
*/

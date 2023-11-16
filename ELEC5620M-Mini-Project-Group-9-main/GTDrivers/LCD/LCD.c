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

#include "LCD.h"

#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"  //some useful delay routines
#include "BasicFont/BasicFont.h"

//
// Driver global static variables (visible only to this .c file)
//

// Driver Base Addresses
volatile unsigned int *lcd_pio_ptr = 0x0;       // 0xFF200060
volatile unsigned short *lcd_hwbase_ptr = 0x0;  // 0xFF200080
// Driver Initialised
bool lcd_initialised = false;

// Store pixel contents of screen in a variable
unsigned short screen[LCD_WIDTH * LCD_HEIGHT];

//
// Useful Defines
//

// Globally define this macro to enable the Hardware Optimised mode.
// #define HARDWARE_OPTIMISED

// PIO Bit Map
#define LCD_WRn (1 << 16)
#define LCD_RS (1 << 17)
#define LCD_RDn (1 << 18)
#define LCD_CSn (1 << 19)
#define LCD_RESETn (1 << 20)
#define LCD_LCD_ON (1 << 21)
#define LCD_HW_OPT (1 << 23)
#define LCD_CMDDATMASK (LCD_CSn | LCD_RDn | LCD_RS | LCD_WRn | 0x0000FFFF)  // CMD and Data bits in PIO

// LCD Dedicated Address Offsets
#define LCD_DEDCMD (0x00 / sizeof(unsigned short))
#define LCD_DEDDATA (0x02 / sizeof(unsigned short))

// LCD PIO Address Offsets
#define LCD_PIO_DATA (0x00 / sizeof(unsigned int))
#define LCD_PIO_DIR (0x04 / sizeof(unsigned int))

// Display Initialisation Data
// You don't need to worry about what all these registers are.
// The LCD LCDs are complicated things with many settings that need
// to be configured - Contrast/Brightness/Data Format/etc.
#define LCD_INIT_DATA_LEN (sizeof(LCD_initData) / sizeof(LCD_initData[0]))
unsigned short LCD_initData[][2] = {
    // isDat, value
    {false, 0x00EF},
    {true, 0x0003},
    {true, 0x0080},
    {true, 0X0002},
    {false, 0x00CF},
    {true, 0x0000},
    {true, 0x0081},
    {true, 0x00C0},
    {false, 0x00ED},
    {true, 0x0064},
    {true, 0x0003},
    {true, 0X0012},
    {true, 0X0081},
    {false, 0x00E8},
    {true, 0x0085},
    {true, 0x0001},
    {true, 0x0078},
    {false, 0x00CB},
    {true, 0x0039},
    {true, 0x002C},
    {true, 0x0000},
    {true, 0x0034},
    {true, 0x0002},
    {false, 0x00F7},
    {true, 0x0020},
    {false, 0x00EA},
    {true, 0x0000},
    {true, 0x0000},
    // Power control
    {false, 0x00C0},
    {true, 0x0023},  // VRH[5:0]
    {false, 0x00C1},
    {true, 0x0010},  // SAP[2:0];BT[3:0]
    // VCM control
    {false, 0x00C5},
    {true, 0x003E},
    {true, 0x0028},
    {false, 0x00C7},
    {true, 0x0086},
    // Memory Access Control (MADCTL)
    {false, 0x0036},
    {true, 0x0048},
    // More settings...
    {false, 0x003A},
    {true, 0x0055},
    {false, 0x00B1},
    {true, 0x0000},
    {true, 0x001B},
    {false, 0x00B6},
    {true, 0x0008},  // Non-Display Area Inaccessible
    {true, 0x0082},  // Normally White, Normal Scan Direction (A2 = Reverse Scan Direction)
    {true, 0x0027},  // 320 Lines
    // 3-Gamma Function Disable
    {false, 0x00F2},
    {true, 0x0000},
    // Gamma curve selected
    {false, 0x0026},
    {true, 0x0001},
    // Set Gamma
    {false, 0x00E0},
    {true, 0x000F},
    {true, 0x0031},
    {true, 0x002B},
    {true, 0x000C},
    {true, 0x000E},
    {true, 0x0008},
    {true, 0x004E},
    {true, 0X00F1},
    {true, 0x0037},
    {true, 0x0007},
    {true, 0x0010},
    {true, 0x0003},
    {true, 0x000E},
    {true, 0x0009},
    {true, 0x0000},
    {false, 0x00E1},
    {true, 0x0000},
    {true, 0x000E},
    {true, 0x0014},
    {true, 0x0003},
    {true, 0x0011},
    {true, 0x0007},
    {true, 0x0031},
    {true, 0x00C1},
    {true, 0x0048},
    {true, 0x0008},
    {true, 0x000F},
    {true, 0x000C},
    {true, 0x0031},
    {true, 0x0036},
    {true, 0x000f},
    // Frame Rate
    {false, 0x00B1},
    {true, 0x0000},
    {true, 0x0001},
    // Interface Control
    {false, 0x00F6},
    {true, 0x0001},
    {true, 0x0010},
    {true, 0x0000},
    // Disable Internal Sleep
    {false, 0x0011},
};

// Helper Methods
// Swap two coordinates using pointers
void swapCoordinates(int (*a)[2], int (*b)[2]) {
    // Store a in a temp
    int temp[2];
    temp[0] = (*a)[0];
    temp[1] = (*a)[1];
    // Set a to b
    (*a)[0] = (*b)[0];
    (*a)[1] = (*b)[1];
    // Set b to temp i.e. a
    (*b)[0] = temp[0];
    (*b)[1] = temp[1];
}

// Sort a list of coordinates using bubble sort
void sortCoordinates(int array[][2], int rows, int key) {
	int i, j;
    // Flag to check if any swaps are made.
    bool swapped;

    // For each element
    for (i = 0; i < rows - 1; i++) {
        // Default value for swap is false
        swapped = false;

        // Pass over all other elements
        for (j = 0; j < rows - i - 1; j++) {
            // If element is larger than the next swap
            if (array[j][key] > array[j + 1][key]) {
                swapCoordinates(&array[j], &array[j + 1]);
                // Set swapped to true
                swapped = true;
            }
        }
        // If no elements have been swapped, list is sorted
        if (!swapped)
            break;
    }
}

signed int LCD_initialise(unsigned int pio_base_address, unsigned int pio_hw_base_address) {
    unsigned int regVal;
    unsigned int idx;

    // Set the local base address pointers
    lcd_pio_ptr = (unsigned int *)pio_base_address;
    lcd_hwbase_ptr = (unsigned short *)pio_hw_base_address;

    // Initialise LCD PIO direction
    // Read-Modify-Write
    regVal = lcd_pio_ptr[LCD_PIO_DIR];                                          // Read
    regVal = regVal | (LCD_CMDDATMASK | LCD_LCD_ON | LCD_RESETn | LCD_HW_OPT);  // All data/cmd bits are outputs
    lcd_pio_ptr[LCD_PIO_DIR] = regVal;                                          // Write

    // Initialise LCD data/control register.
    // Read-Modify-Write
    regVal = lcd_pio_ptr[LCD_PIO_DATA];                                          // Read
    regVal = regVal & ~(LCD_CMDDATMASK | LCD_LCD_ON | LCD_RESETn | LCD_HW_OPT);  // Mask all data/cmd bits
    regVal = regVal | (LCD_CSn | LCD_WRn | LCD_RDn);                             // Deselect Chip and set write and read signals to idle.
#ifdef HARDWARE_OPTIMISED
    regVal = regVal | LCD_HW_OPT;  // Enable HW opt bit.
#endif
    lcd_pio_ptr[LCD_PIO_DATA] = regVal;  // Write

    // LCD requires specific reset sequence:
    LCD_powerConfig(true);  // turn on for 1ms
    usleep(1000);
    LCD_powerConfig(false);  // then off for 10ms
    usleep(10000);
    LCD_powerConfig(true);  // finally back on and wait 120ms for LCD to power on
    usleep(120000);

    // Upload Initialisation Data
    for (idx = 0; idx < LCD_INIT_DATA_LEN; idx++) {
        LCD_write(LCD_initData[idx][0], LCD_initData[idx][1]);
    }

    // Allow 120ms time for LCD to wake up
    usleep(120000);

    // Turn on display drivers
    LCD_write(false, 0x0029);

    // Mark as initialised so later functions know we are ready
    lcd_initialised = true;

    // And clear the display
    return LCD_clearDisplay(LCD_BLACK);
}

// Check if driver initialised
bool LCD_isInitialised() {
    return lcd_initialised;
}

#ifdef HARDWARE_OPTIMISED
// If HARDWARE_OPTIMISED is defined, use this optimised function. It will be discussed in the lab on LCDs.

// Function for writing to LCD Registers (using dedicated HW)
// You must check LCD_isInitialised() before calling this function
void LCD_write(bool isData, unsigned short value) {
    if (isData) {
        lcd_hwbase_ptr[LCD_DEDDATA] = value;
    } else {
        lcd_hwbase_ptr[LCD_DEDCMD] = value;
    }
}

#else
// Otherwise use the non-optimised function.

// Function for writing to LCD Registers (using PIO)
// You must check LCD_isInitialised() before calling this function
void LCD_write(bool isData, unsigned short value) {
    // PIO controls more than just LCD, so need to Read-Modify-Write
    // First we have to output the value with the LCD_WRn bit low (first cycle of write)
    // Read
    unsigned int regVal = lcd_pio_ptr[LCD_PIO_DATA];
    // Modify
    // Mask all bits for command and data (sets them all to 0)
    regVal = regVal & ~LCD_CMDDATMASK;
    // Set the data bits (unsigned value, so cast pads MSBs with 0's)
    regVal = regVal | ((unsigned int)value);
    if (isData) {
        // For data we set the RS bit high.
        regVal = regVal | (LCD_RS | LCD_RDn);
    } else {
        // For command we don't set the RS bit
        regVal = regVal | (LCD_RDn);
    }
    // Write
    lcd_pio_ptr[LCD_PIO_DATA] = regVal;
    // Then we need to output the value again with LCD_WRn high (second cycle of write)
    // Rest of regVal is unchanged, so we just or on the LCD_WRn bit
    regVal = regVal | (LCD_WRn);
    // Write
    lcd_pio_ptr[LCD_PIO_DATA] = regVal;
}

#endif

// Function for configuring LCD reset/power (using PIO)
// You must check LCD_isInitialised() before calling this function
void LCD_powerConfig(bool isOn) {
    // Read
    unsigned int regVal = lcd_pio_ptr[LCD_PIO_DATA];
    // Modify
    if (isOn) {
        // To turn on we must set the RESETn and LCD_ON bits high
        regVal = regVal | (LCD_RESETn | LCD_LCD_ON);
    } else {
        // To turn off we must set the RESETn and LCD_ON bits low
        regVal = regVal & ~(LCD_RESETn | LCD_LCD_ON);
    }
    // Write
    lcd_pio_ptr[LCD_PIO_DATA] = regVal;
}

// Function to set the display to a color
//  - Returns true if successful
signed int LCD_clearDisplay(unsigned short colour) {
    signed int status;
    unsigned int idx;
    // Reset watchdog.
    ResetWDT();
    // Define window as entire display (LCD_setWindow will check if we are initialised).
    status = LCD_setWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
    if (status != LCD_SUCCESS)
        return status;
    // Loop through each pixel in the window writing the required colour
    for (idx = 0; idx < (LCD_WIDTH * LCD_HEIGHT); idx++) {
        LCD_write(true, colour);
    }
    // And done.
    return LCD_SUCCESS;
}

// Function to convert Red/Green/Blue to RGB565 encoded colour value
unsigned short LCD_makeColour(unsigned int R, unsigned int G, unsigned int B) {
    unsigned short colour;
    // Limit the colours to the maximum range
    if (R > 0x1F)
        R = 0x1F;
    if (G > 0x3F)
        G = 0x3F;
    if (B > 0x1F)
        B = 0x1F;
    // Move the RGB values to the correct place in the encoded colour
    colour = (R << 11) + (G << 5) + (B << 0);
    return colour;
}

// Function to set the drawing window on the display
//   Returns 0 if successful
signed int LCD_setWindow(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height) {
    unsigned int xright, ybottom;
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;  // Don't run if not yet initialised
    // Calculate bottom right corner location
    xright = xleft + width - 1;
    ybottom = ytop + height - 1;
    // Ensure end coordinates are in range
    if (xright >= LCD_WIDTH)
        return LCD_INVALIDSIZE;  // Invalid size
    if (ybottom >= LCD_HEIGHT)
        return LCD_INVALIDSIZE;  // Invalid size
    // Ensure start coordinates are in range (top left must be <= bottom right)
    if (xleft > xright)
        return LCD_INVALIDSHAPE;  // Invalid shape
    if (ytop > ybottom)
        return LCD_INVALIDSHAPE;  // Invalid shape
    // Define the left and right of the display
    LCD_write(false, 0x002A);
    LCD_write(true, (xleft >> 8) & 0xFF);
    LCD_write(true, xleft & 0xFF);
    LCD_write(true, (xright >> 8) & 0xFF);
    LCD_write(true, xright & 0xFF);
    // Define the top and bottom of the display
    LCD_write(false, 0x002B);
    LCD_write(true, (ytop >> 8) & 0xFF);
    LCD_write(true, ytop & 0xFF);
    LCD_write(true, (ybottom >> 8) & 0xFF);
    LCD_write(true, ybottom & 0xFF);
    // Create window and prepare for data
    LCD_write(false, 0x002c);
    // Done
    return LCD_SUCCESS;
}

// Copy frame buffer to display
//  - returns 0 if successful
signed int LCD_copyFrameBuffer(const unsigned short *framebuffer, unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height) {
    unsigned int cnt;
    // Define Window
    signed int status = LCD_setWindow(xleft, ytop, width, height);
    if (status != LCD_SUCCESS)
        return status;
    // And Copy
    cnt = (height * width);  // How many pixels.
    while (cnt--) {
        LCD_write(true, *framebuffer++);
    }
    // Done
    return LCD_SUCCESS;
}

// Set the value of a pixel on the display
// Modifies 'screen' array.
signed int LCD_drawPixel(unsigned int x, unsigned int y, unsigned short color) {
	int pixel_id;
    // Check if the LCD is initialized
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Covert x,y coordinates to index in 'screen' array
    pixel_id = y * LCD_WIDTH + x;
    // Set the color of the element / pixel
    screen[pixel_id] = color;

    // Done
    return LCD_SUCCESS;
}

// Function to set the color of the screen
signed int LCD_setColor(unsigned int R, unsigned int G, unsigned int B) {
	unsigned short lt24colour;
    int pixel;

    // Check if the LCD is initialized
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Get the color in RGB565 encoded value
    lt24colour = LCD_makeColour(R, G, B);

    // Fill every pixel on LCD display with the color
    for (pixel = 0; pixel < LCD_HEIGHT * LCD_WIDTH; pixel++) {
        screen[pixel] = lt24colour;
    }

    // Done
    return LCD_SUCCESS;
}

// Function to draw a line with specified color.
signed int LCD_drawLine(int x1, int y1, int x2, int y2, unsigned short color) {
	int dx, dy, sx, sy, err, e2;
    // Check if the LCD is initialized before drawing anything
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Draw Line using Bresenham's Algorithm
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;

    while (true) {
        LCD_drawPixel(x1, y1, color);
        // Stop if we reach the end point
        if (x1 == x2 && y1 == y2)
            break;

        // Calculate error term and move to next pixel
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    // Done
    return LCD_SUCCESS;
}

// Function to draw a triangle with specified color and fill.
signed int LCD_drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned short color, bool fill) {
    // Create a list of x,y pairs
	const int num_vertices = 3;
    int points[3][2];
    int top_x, mid_x, bot_x, top_y, mid_y, bot_y;
    float slope1, slope2, c1, c2;
    int current_y, p1X, p1Y, p2X, p2Y;

    points[0][0] = x1;
    points[0][1] = y1;
    points[1][0] = x2;
    points[1][1] = y2;
    points[2][0] = x3;
    points[2][1] = y3;

	// Check if the LCD is initialized before drawing anything
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;
    // First, draw the outline of the triangle
    LCD_drawLine(x1, y1, x2, y2, color);
    LCD_drawLine(x2, y2, x3, y3, color);
    LCD_drawLine(x3, y3, x1, y1, color);

    // If fill is 1, fill in the triangle
    if (fill) {
        // Sort the list in ascending order by y-value
        sortCoordinates(points, num_vertices, 1);

        // Determine top, middle and bottom vertices
        top_x = points[0][0], top_y = points[0][1];
        mid_x = points[1][0], mid_y = points[1][1];
        bot_x = points[2][0], bot_y = points[2][1];

        // Calculate the slopes of the two sides of the triangle
        // Using equation y = mx + c
        slope1 = (bot_x - top_x) == 0 ? 0 : (float)(bot_y - top_y) / (bot_x - top_x);
        c1 = top_y - (top_x * slope1);
        slope2 = (mid_x - top_x) == 0 ? 0 : (float)(mid_y - top_y) / (mid_x - top_x);
        c2 = top_y - (top_x * slope2);

        // Loop over every y-coordinate on edges till lowest point.
        for (current_y = top_y; current_y <= max(bot_y, mid_y); current_y++) {
            // Use slope1 and c1 to calculate point 1's x-coord
            p1X = (int)((current_y - c1) / slope1 + 0.5f);
            // y-coord cannot be lower than bot_Y or top_y
            p1Y = min(max(current_y, top_y), bot_y);

            // Use slope2 and c2 to calculate point 2's x-coord
            p2X = (int)((current_y - c2) / slope2 + 0.5f);
            // y-coord cannot be lower than mid_y or top_y
            p2Y = min(max(current_y, top_y), mid_y);

            // Draw a line between corresponding points on both edges.
            LCD_drawLine(p1X, p1Y, p2X, p2Y, color);
        }
    }

    // Done
    return LCD_SUCCESS;
}

// Draws a rectangle on the LCD display with top-left at (x0, y0) with the
// specified height, width and color. If fill is non-zero, fills the circle
// with the specified color, otherwise only the outline is drawn
signed int LCD_drawRectangle(int x, int y, int height, int width, unsigned short color, bool fill) {
	int line_y;
    // Check if the LCD is initialized before drawing anything
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Draw the outline of the rectangle
    LCD_drawLine(x, y, x + height, y, color);
    LCD_drawLine(x + height, y, x + height, y + width, color);
    LCD_drawLine(x + height, y + width, x, y + width, color);
    LCD_drawLine(x, y + width, x, y, color);

    // If fill is 1, fill in the rectangle
    if (fill) {
        // For each point on the edge,
        // fill in the pixels between the left and right edges of the rectangle
        for (line_y = y + 1; line_y < y + width; line_y++) {
            LCD_drawLine(x + 1, line_y, x + height - 1, line_y, color);
        }
    }

    // Done
    return LCD_SUCCESS;
}

// Draws a circle on the LCD display centered at (x0, y0) with the specified radius and color
// If fill is non-zero, fills the circle with the specified color, otherwise only the outline is drawn
signed int LCD_drawCircle(int x0, int y0, int radius, unsigned short color, bool fill) {
	int x,y,error, x1, y1, x2, y2, x3, y3, x4, y4;
    // Check if the LCD is initialized before drawing anything
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Use the Midpoint Circle Algorithm to draw and fill the circle
    x = radius;
    y = 0;
    error = 0;

    while (x >= y) {
        // Calculate the coordinates of the four points on the circle
        x1 = x0 - x;
        y1 = y0 - y;
        x2 = x0 + x;
        y2 = y0 + y;
        x3 = x0 - y;
        y3 = y0 - x;
        x4 = x0 + y;
        y4 = y0 + x;

        if (fill) {
            // If filling, draw lines horizontally across the circle in quarters
            LCD_drawLine(x1, y1, x1, y2, color);  // Top quarter
            LCD_drawLine(x3, y3, x3, y4, color);  // Top middle quarter
            LCD_drawLine(x4, y3, x4, y4, color);  // Bottom middle quarter
            LCD_drawLine(x2, y1, x2, y2, color);  // Bottom quarter
        } else {
            // If not filling, draw individual pixels at the circle's outline
            LCD_drawPixel(x2, y2, color);
            LCD_drawPixel(x4, y4, color);
            LCD_drawPixel(x3, y4, color);
            LCD_drawPixel(x1, y2, color);
            LCD_drawPixel(x1, y1, color);
            LCD_drawPixel(x3, y3, color);
            LCD_drawPixel(x4, y3, color);
            LCD_drawPixel(x2, y1, color);
        }

        // Update the error value and the y value based on whether we've moved past the midpoint of the circle
        if (error <= 0) {
            y += 1;
            error += 2 * y + 1;
        }

        // Update the x value and error value based on whether we've moved past the midpoint of the circle
        if (error > 0) {
            x -= 1;
            error -= 2 * x + 1;
        }
    }

    // Done
    return LCD_SUCCESS;
}

signed int LCD_drawChar(char character, int x, int y, unsigned short color, int size) {
	int row, col, i, j;
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // For every row in 8x5 bitmap image
    for (row = 0; row < 5; row++) {
        // Get the row
        signed char c = BF_fontMap[character - ' '][row];

        // Loop over columns
        for (col = 7; col >= 0; col--) {
            // If bit is set, color the pixel and scale it up
            if ((c >> col) & 1) {
                // Scale pixels in both x and y directions
                for (i = 0; i < size; i++) {
                    for (j = 0; j < size; j++) {
                        LCD_drawPixel(x + ((7 - col) * size) + i, y + (row * size) + j, color);
                    }
                }
            }
        }

        // Done
    }
    return LCD_SUCCESS;
}

signed int LCD_drawText(char text[], int x, int y, unsigned short color, int size) {
	int i;

    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Loop over each character and draw it with space in between
    for (i = 0; i < strlen(text); i++) {
        LCD_drawChar(text[i], x, y + (8 * size * i), color, size);
    }

    // Done
    return LCD_SUCCESS;
}

signed int LCD_update() {
	signed int status;
	int id, length;
    if (!LCD_isInitialised())
        return LCD_ERRORNOINIT;

    // Set full display as window
    status = LCD_setWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
    length = LCD_HEIGHT * LCD_WIDTH;

    if (status != LCD_SUCCESS)
        return status;

    // For each pixel in 'screen' array
    for (id = 0; id < length; id++) {
        // Get color for pixel
        unsigned short color = screen[id];
        // Write pixel to display.
        LCD_write(true, color);
    }

    // Done
    return LCD_SUCCESS;
}

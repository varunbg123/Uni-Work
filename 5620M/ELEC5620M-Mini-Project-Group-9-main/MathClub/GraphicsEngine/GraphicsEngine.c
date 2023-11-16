/*
 * MathClub Game Graphics Engine
 * ------------------------------
 * Description:
 * Graphics Engine for MathClub Game
 * Provides methods to draw screens and widgets
 *
 * Company: University of Leeds
 * Author: Kaif Kutchwala
 * Emmanuel Leo and Varun Gonsalves
 *
 */

#include "GraphicsEngine.h"

#include "LCD/LCD.h"

unsigned int RED[3] = {255u, 0u, 0u};
unsigned int GREEN[3] = {0u, 255u, 0u};
unsigned int BLUE[3] = {0u, 0u, 255u};
unsigned int YELLOW[3] = {255u, 255u, 0u};
unsigned int MAGENTA[3] = {255u, 0u, 255u};
unsigned int CYAN[3] = {0u, 255u, 255u};
unsigned int ORANGE[3] = {255u, 128u, 0u};
unsigned int PURPLE[3] = {128u, 0u, 128u};
unsigned int GRAY[3] = {128u, 128u, 128u};
unsigned int DARK_RED[3] = {128u, 0u, 0u};
unsigned int DARK_GREEN[3] = {0u, 128u, 0u};
unsigned int DARK_BLUE[3] = {0u, 0u, 128u};
unsigned int DARK_YELLOW[3] = {128u, 128u, 0u};
unsigned int DARK_MAGENTA[3] = {128u, 0u, 128u};
unsigned int DARK_CYAN[3] = {0u, 128u, 128u};
unsigned int LIGHT_GRAY[3] = {192u, 192u, 192u};
unsigned int DARK_GRAY[3] = {64u, 64u, 64u};
unsigned int BLACK[3] = {0u, 0u, 0u};
unsigned int WHITE[3] = {255u, 255u, 255u};

// Store colors for multiple choice options
unsigned short option_colors[4] = {LCD_BLUE, LCD_RED, LCD_BLACK, LCD_MAGENTA};
// Store volume bar color as white
unsigned int volume_bar_color[3] = {255, 255, 255};

// Helper method: Returns width of text in pixel depending on size.
float calcTextWidth(char* text, double size) {
    // Size 1 is 7 (5 + 2 spaces) pixels, size 2 is 14 pixels and so on.
    float character_width = (7) * size;
    // Multiply character width by number of characters
    return strlen(text) * character_width;
}

// Light wrapper around LCD_setColor to set background color
void GraphicsEngine_setBackground(unsigned int R, unsigned int G, unsigned int B) {
    LCD_setColor(R, G, B);
}

// Draws a sound icon at specified x,y (bottom-left) coordinates
void GraphicsEngine_drawSoundIcon(unsigned int x, unsigned int y) {
    LCD_drawTriangle(x, y + 15, x + 16, y + 14, x + 8, y, LCD_WHITE, false);
    LCD_drawLine(x + 7, y + 19, x + 9, y + 19, LCD_WHITE);
    LCD_drawLine(x + 4, y + 24, x + 12, y + 24, LCD_WHITE);
    LCD_drawLine(x, y + 28, x + 16, y + 28, LCD_WHITE);
}

// Draws a progress bar at specificed x,y (bottom-left), width area proportional to
// `value` filled.
void GraphicsEngine_drawProgressBar(unsigned int x, unsigned int y, float height, float width, float value, unsigned int color[3]) {
    // Convert color to RGB565 enoded value
    unsigned short pb_color = LCD_makeColour(color[0], color[1], color[2]);
    // Calculate width of area to fill, max value is 100
    float progress_width = width * (value / 100);

    // Draw outline of progress bar
    LCD_drawRectangle(x, y, height, width, pb_color, false);
    // Fill area proportional to `value` in progress bar
    LCD_drawRectangle(x, y, height, progress_width, pb_color, true);
}

// Draws the question on the screen
void GraphicsEngine_drawQuestion(char* text) {
    // draw the text at fixed location for question.
    LCD_drawText(text, 150, 40, LCD_BLACK, 2);
}

// Draws a multiple choice options on the screen
// Option number specifies which option it is
void GraphicsEngine_drawOption(char* text, unsigned int option_number) {
    // Determine x,y coordinates depending on option number
    // Options are displayed as follows
    //        0                        1
    //        2                        3
    int x = option_number == 0 || option_number == 1 ? 85 : 25;
    int y = option_number == 0 || option_number == 2 ? 40 : 170;

    // Choose a size so that text can fit the rectangle
    int text_size = strlen(text) <= 3 ? 3 : strlen(text) < 6 ? 2 : 1;
    // Select padding based on text size
    int padding_x = text_size == 1 ? 20 : text_size == 2 ? 15 : 10;
    int padding_y = 10;

    // Draw rectangle with text inside
    LCD_drawRectangle(x, y, 45, 110, option_colors[option_number], true);
    LCD_drawText(text, x + padding_x, y + padding_y, LCD_WHITE, text_size);
}

// Draws a message on the screen
// Covers the screen with the background_color, draws a circle with the shape_color
// Displays text in the circle.
void GraphicsEngine_drawMessage(char* text, unsigned int text_color[3], unsigned int background_color[3], unsigned int shape_color[3]) {
    // Convert colors to RBG565 encoded values
    unsigned short txt_color = LCD_makeColour(text_color[0], text_color[1], text_color[2]);
    unsigned short shp_color = LCD_makeColour(shape_color[0], shape_color[1], shape_color[2]);
    int text_size = 3;

    // Set bg color, draw circle and add text
    LCD_setColor(background_color[0], background_color[1], background_color[2]);
    LCD_drawCircle((int)(LCD_WIDTH / 2), (int)(LCD_HEIGHT / 2), 110, shp_color, true);
    LCD_drawText(text, (int)((LCD_WIDTH / 2) - 15), (int)((LCD_HEIGHT / 2) - (calcTextWidth(text, text_size) / 2)), txt_color, text_size);
}

void GraphicsEngine_drawVolumeBar(unsigned int volume, unsigned int x, unsigned int y, unsigned int height, unsigned int width) {
    // Volume cannot be greater than 10
    if (volume > 10)
        volume = 10;

    // Draw sound icon and use progress bar to show volume level
    GraphicsEngine_drawSoundIcon(x, y);
    GraphicsEngine_drawProgressBar(x, y + 45, height, width, volume * 10, volume_bar_color);
}

// Draws main menu page
void GraphicsEngine_drawMainMenu(bool is_hard, unsigned int volume, unsigned int high_score) {
    char* highscore;
    // Set background to BLACK
    GraphicsEngine_setBackground(0, 0, 0);

    // Draw logo at the top of the screen
    GraphicsEngine_drawLogo(180, 50);

    // Add Highscore text
    sprintf(highscore, "%u", high_score);
    LCD_drawText("High score:", 125, 50, LCD_WHITE, 2);
    LCD_drawText(highscore, 125, 250, LCD_BLUE, 2);

    // Add "Play" text
    LCD_drawText("Play   B0", 90, 100, LCD_WHITE, 2);

    // If level is hard, add "Hard SW0" text in red else "Easy SW0" in green
    if (is_hard) {
        LCD_drawText("Hard  SW0", 60, 100, LCD_RED, 2);
    } else {
        LCD_drawText("Easy  SW0", 60, 100, LCD_GREEN, 2);
    }

    // Draw volume bar
    GraphicsEngine_drawVolumeBar(volume, 15, 15, 15, 240);
}

// Draws Pause Menu
void GraphicsEngine_drawPauseMenu(unsigned int volume) {
    // Set background to BLACK
    GraphicsEngine_setBackground(0,0,0);
    
    // Draw a yellow pause icon at the top of the screen
    LCD_drawRectangle(170, 145, 35, 10, LCD_YELLOW, true);
    LCD_drawRectangle(170, 165, 35, 10, LCD_YELLOW, true);

    // Add "Return B0" text
    LCD_drawText("Return  B0", 120, (int)((LCD_WIDTH / 2) - (calcTextWidth("Return  B0", 2) / 2)), LCD_WHITE, 2);
    // Add "Exit B1" text
    LCD_drawText("Exit   SW9", 80, (int)((LCD_WIDTH / 2) - (calcTextWidth("Exit   SW9", 2) / 2)), LCD_RED, 2);

    // Draw volume bar
    GraphicsEngine_drawVolumeBar(volume, 15, 15, 15, 240);
}

// Draws MathClub Logo at specified x,y (bottom-left)
void GraphicsEngine_drawLogo(unsigned int x, unsigned int y) {
    // Draw red rectangle, blue triangle and yellow circle
    LCD_drawRectangle(x - 5, y, 40, 30, LCD_RED, true);
    LCD_drawTriangle(x + 8, y + 2, x, y - 15, x - 14, y + 15, LCD_BLUE, true);
    LCD_drawCircle(x + 10, y + 30, 10, LCD_YELLOW, true);
    // Add "MathClub" text
    LCD_drawText("MathClub", x, y + 50, LCD_WHITE, 3);
}

// Light wrapper around LCD_update to update contents of screen
void GraphicsEngine_update() {
    LCD_update();
}

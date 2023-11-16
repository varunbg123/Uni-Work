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

#ifndef GraphicsEngine_H_
#define GraphicsEngine_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern unsigned int RED[3];
extern unsigned int GREEN[3];
extern unsigned int BLUE[3];
extern unsigned int YELLOW[3];
extern unsigned int MAGENTA[3];
extern unsigned int CYAN[3];
extern unsigned int ORANGE[3];
extern unsigned int PURPLE[3];
extern unsigned int GRAY[3];
extern unsigned int DARK_RED[3];
extern unsigned int DARK_GREEN[3];
extern unsigned int DARK_BLUE[3];
extern unsigned int DARK_YELLOW[3];
extern unsigned int DARK_MAGENTA[3];
extern unsigned int DARK_CYAN[3];
extern unsigned int LIGHT_GRAY[3];
extern unsigned int DARK_GRAY[3];
extern unsigned int BLACK[3];
extern unsigned int WHITE[3];

/**
 * GraphicsEngine_setBackground
 *
 * Sets the background color of the screen.
 * Light wrapper around LCD_setColor
 *
 */
void GraphicsEngine_setBackground(unsigned int R, unsigned int G, unsigned int B);

/**
 * GraphicsEngine_drawSoundIcon
 *
 * Draws a sound/volume icon at specified x,y (bottom-left)
 *
 * Inputs:
 *      x, y:        x,y coordinates of bottom-left of icon
 *
 */
void GraphicsEngine_drawSoundIcon(unsigned int x, unsigned int y);

/**
 * GraphicsEngine_drawProgressBar
 *
 * Draws a progress bar widget on the screen at specified x,y with
 * area proportional to `value` filled and specified color.
 *
 * Inputs:
 *      x,y:        coordinates of bottom left of progress bar
 *      height:     height of progress bar
 *      width:      width of progress bar
 *      value:      value of progress bar out of 100
 *      color:      color of progress bar in R,G,B
 *
 */
void GraphicsEngine_drawProgressBar(unsigned int x, unsigned int y, float height, float width, float value, unsigned int color[3]);

/**
 * GraphicsEngine_drawQuestion
 *
 * Draws the question on the screen.
 *
 * Inputs:
 *      text:       the text content of the question
 *
 */
void GraphicsEngine_drawQuestion(char* text);

/**
 * GraphicsEngine_drawOption
 *
 * Draws a multiple choice options on the screen
 * Option number specifies which option it is
 * Options are displayed as follows
 *        0                        1
 *        2                        3
 *
 * Inputs:
 *      text:               the text content of the option
 *      option_number:      the option number (0-3)
 *
 */
void GraphicsEngine_drawOption(char* text, unsigned int option_number);

/**
 * GraphicsEngine_drawMessage
 *
 * Displays a message covering the entire screen.
 * Screen is covered with background color, a large circle is drawn
 * with shape color and text is written in the shape with text color.
 *
 * Inputs:
 *      text:               the text content of the message
 *      text_color:         color of the text in R,G,B
 *      background_color:   color of the background in R,G,B
 *      shape_color:        color of the shape/circle in R,G,B
 *
 */
void GraphicsEngine_drawMessage(char* text, unsigned int text_color[3], unsigned int background_color[3], unsigned int shape_color[3]);

/**
 * GraphicsEngine_drawVolumeBar
 *
 * Draws a volume bar displaying the specified volume.
 *
 * Inputs:
 *      volume:    int to represent the volume level (1-10).
 *      x,y:       x,y coordinates of bottom left of volume bar.
 *      height:    height of volume bar
 *      width:     width of volume bar
 *
 */
void GraphicsEngine_drawVolumeBar(unsigned int volume, unsigned int x, unsigned int y, unsigned int height, unsigned int width);

/**
 * GraphicsEngine_drawMainMenu
 *
 * Draws the main menu for the MathClub Game
 * Includes drawing the logo, "Play" option, difficulty level
 * high score and volume level.
 *
 * Inputs:
 *      is_hard:       bool to represent mode - true = hard, false = easy
 *      volume:        int to represent the volume level (1-10).
 *      high_score:    the high score to display as an int
 *
 */
void GraphicsEngine_drawMainMenu(bool is_hard, unsigned int volume, unsigned int high_score);

/**
 * GraphicsEngine_drawPauseMenu
 *
 * Draws the pause menu for the MathClub Game
 * Includes drawing a pause icon, "Return" option, "Exit" option
 * and volume level.
 *
 * Inputs:
 *      volume:        int to represent the volume level (1-10).
 *
 */
void GraphicsEngine_drawPauseMenu(unsigned int volume);

/**
 * GraphicsEngine_drawLogo
 *
 * Draws the MathClub logo at specified x,y (bottom-left)
 *
 * Inputs:
 *      x, y:        x,y coordinates of bottom-left of logo
 *
 */
void GraphicsEngine_drawLogo(unsigned int x, unsigned int y);

/**
 * GraphicsEngine_update
 *
 * Updates the screen contents.
 * Light wrapper around LCD_update
 *
 */
void GraphicsEngine_update(void);

#endif /*GraphicsEngine_H_*/

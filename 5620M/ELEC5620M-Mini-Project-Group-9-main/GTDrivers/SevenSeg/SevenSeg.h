/*
 * SevenSeg.h
 *
 *  Created on: 13 April 2023
 *      Author: Kaif Kutchwala
 */

#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include <math.h>
#include <stdbool.h>

// Status Codes
#define SEVENSEG_SUCCESS 0
#define SEVENSEG_ERRORNOINIT 1

// Useful variables
#define MAX_DISPLAYABLE_CHARACTERS 6

// Useful Functions
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/*
 * SEVENSEG_initialise
 * Set the base address for the Seven segment displays
 */
signed int SevenSeg_initialise(unsigned int lo_base_address, unsigned int hi_base_address);

/*
 * SEVENSEG_isInitialised
 * A check to make sure the Seven segment displays are initialised
 */
bool SevenSeg_isInitialised(void);

/**
 * SevenSeg_write
 *
 * Low level set function to send a byte value to a selected display.
 *
 * Inputs:
 * 		display:		index of the display to update (0-5)
 * 		value: 			byte to set display to.
 *
 * Output: Returns SEVENSEG_SUCCESS if operation completed succesfully
 *         else returns SEVENSEG_ERROR
 */
signed int SevenSeg_write(unsigned int display, unsigned char value);

/**
 * SevenSeg_displayCharacter
 *
 * Display a character on a specified display.
 *
 * Inputs:
 * 		display:		index of the display to update (0-5)
 * 		character:		the character to display
 *
 * Output: Returns SEVENSEG_SUCCESS if operation completed succesfully
 *         else returns SEVENSEG_ERROR
 */
signed int SevenSeg_displayCharacter(unsigned int display, char character);

/**
 * SevenSeg_displayWord
 *
 * Display a word using all 6 seven segment displays from left to right
 *
 * Inputs:
 * 		word:		The word to be displayed (max length = 6)
 *
 * Output: Returns SEVENSEG_SUCCESS if operation completed succesfully
 *         else returns SEVENSEG_ERROR
 */
signed int SevenSeg_displayWord(char word[6]);

/**
 * SevenSeg_displayDigit
 *
 * Display a digit on a specified display.
 *
 * Inputs:
 * 		display:		index of the display to update (0-5)
 * 		number:		    the digit to display (0-9)
 *
 * Output: Returns SEVENSEG_SUCCESS if operation completed succesfully
 *         else returns SEVENSEG_ERROR
 */
signed int SevenSeg_displayDigit(unsigned int display, int number);

/**
 * SevenSeg_displayNumber
 *
 * Display a number using all 6 seven segment displays from left to right.
 * If the number is longer than 6 digits only the last 6 digits are displayed.
 *
 * Inputs:
 * 		number:		   The number to be displayed.
 *
 * Output: Returns SEVENSEG_SUCCESS if operation completed succesfully
 *         else returns SEVENSEG_ERROR
 */
signed int SevenSeg_displayNumber(int number);

#endif /* SEVENSEG_H_ */

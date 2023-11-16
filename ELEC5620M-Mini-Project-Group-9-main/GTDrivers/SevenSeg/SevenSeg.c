/*
 * SevenSeg.c
 *
 *  Created on: 13 April 2023
 *      Author: Kaif Kutchwala
 */

#include "SevenSeg.h"

#include <stdlib.h>

// Base addresses of the seven segment display peripherals.
volatile unsigned char *sevenseg_base_lo_ptr = 0x0;  // 0xFF200020;
volatile unsigned char *sevenseg_base_hi_ptr = 0x0;  // 0xFF200030;
bool sevenseg_initialised = false;

// Stores all characters that can be displayed on the seven segment display
int num_display_characters = 38;
char display_characters[38] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', ' '};

// Stores mappings for displaying display_characters on a seven segment display
// Element index ranges from 0-37
// Element itself is a hex which turns on bits
unsigned char sevenseg_mappings[38] = {
    0x3F,  // 01111111 -> 0
    0x06,  // 00000110 -> 1
    0x5B,  // 01011011 -> 2
    0x4F,  // 01001111 -> 3
    0x66,  // 01100110 -> 4
    0x6D,  // 01101101 -> 5
    0x7D,  // 01111101 -> 6
    0x07,  // 00000111 -> 7
    0x7F,  // 01111111 -> 8
    0x67,  // 01100111 -> 9
    0x77,  // 01110111 -> A
    0x7C,  // 01111100 -> b
    0x39,  // 00111001 -> C
    0x5E,  // 01011110 -> d
    0x79,  // 01111001 -> E
    0x71,  // 01110001 -> F
    0x6F,  // 01101111 -> g
    0x76,  // 01110110 -> H
    0x06,  // 00000110 -> i
    0x1E,  // 00011110 -> j
    0x78,  // 01111000 -> k
    0x38,  // 00111000 -> L
    0x15,  // 00010101 -> M
    0x54,  // 01010100 -> n
    0x5C,  // 01011100 -> o
    0x73,  // 01110011 -> P
    0x67,  // 01100111 -> q
    0x31,  // 00110001 -> r
    0x6D,  // 01101101 -> S
    0x78,  // 01111000 -> t
    0x3E,  // 00111110 -> U
    0x1C,  // 00011100 -> v
    0x2A,  // 00101010 -> W
    0x49,  // 01001001 -> X
    0x6E,  // 01101110 -> y
    0x5B,  // 01011011 -> Z
    0x40,  // 01000000 -> -
    0x00,  // 00000000 -> whitespace
};

// Index value for '-' in sevenseg_mappings
unsigned int dash_index = 36;
// Index value for whitespace in sevenseg_mappings
const unsigned int whitespace_index = 37;

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

// Helper Methods

int findIndex(char array[], int size, char value) {
    int index = 0;  // Start search from first element

    // While index is less than size
    // and element is not eq to search value
    while ((index < size) && (array[index] != value))
        index++;  // Check next element

    // If index is within array size return it else return -1
    return (index < size) ? index : -1;
}

int countNumDigits(int number) {
    int num_digits = (number == 0) ? 1 : log10((double)number) + 1;
    return num_digits;
}

void getDigitsInNumber(int number, int *digits_array) {
    int length = countNumDigits(number);
    int digit_position;

    for (digit_position = 0; digit_position < min(length, MAX_DISPLAYABLE_CHARACTERS); digit_position++) {
        digits_array[digit_position] = number % 10;
        number /= 10;
    }
}

// Initialise driver
signed int SevenSeg_initialise(unsigned int lo_base_address, unsigned int hi_base_address) {
    // Set the local base address pointers
    sevenseg_base_lo_ptr = (unsigned char *)lo_base_address;
    sevenseg_base_hi_ptr = (unsigned char *)hi_base_address;

    // Set bool as initialised
    sevenseg_initialised = true;

    return SEVENSEG_SUCCESS;
}

// Check if driver initialised
bool SevenSeg_isInitialised() {
    return sevenseg_initialised;
}

// Low level set function to send a byte value to a selected display.
signed int SevenSeg_write(unsigned int display, unsigned char value) {
    // Return error code if driver not initialised
    if (!SevenSeg_isInitialised()) return SEVENSEG_ERRORNOINIT;

    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }

    return SEVENSEG_SUCCESS;
}

// Display a character on the specified display.
signed int SevenSeg_displayCharacter(unsigned int display, char character) {
    int character_index;
    unsigned char display_value;

    // Return error code if driver not initialised
    if (!SevenSeg_isInitialised()) return SEVENSEG_ERRORNOINIT;

    // Find the index of character from display characters
    character_index = findIndex(display_characters, num_display_characters, character);

    // If value is found find corresponding seven segment mapping
    // Else set display value to mapping for whitespace
    display_value = sevenseg_mappings[(character_index == -1) ? whitespace_index : character_index];

    // Display character on the display starting from left
    SevenSeg_write(display, display_value);
    return SEVENSEG_SUCCESS;
}

// Display a word using all 6 seven segment displays from left to right
signed int SevenSeg_displayWord(char word[6]) {
    int loop_index;

    // Return error code if driver not initialised
    if (!SevenSeg_isInitialised()) return SEVENSEG_ERRORNOINIT;

    // Loop through each character in word
    for (loop_index = 0; loop_index < 6; loop_index++) {
        // Store hex value and
        // Display character on the display starting from left
        SevenSeg_displayCharacter(5 - loop_index, (char)word[loop_index]);
    }
    return SEVENSEG_SUCCESS;
}

// Displays a digit on the specified display.
signed int SevenSeg_displayDigit(unsigned int display, int digit) {
    unsigned char display_value;

    // Return error code if driver not initialised
    if (!SevenSeg_isInitialised()) return SEVENSEG_ERRORNOINIT;

    display_value = sevenseg_mappings[digit];
    SevenSeg_write(display, display_value);

    return SEVENSEG_SUCCESS;
}

// Display a number using all 6 seven segment displays from left to right.
// If the number is longer than 6 digits only the last 6 digits are displayed.
signed int SevenSeg_displayNumber(int number) {
    // Initialise digits to all be white space characters
    int digits[MAX_DISPLAYABLE_CHARACTERS] = {whitespace_index, whitespace_index, whitespace_index, whitespace_index, whitespace_index, whitespace_index};
    int num_digits = countNumDigits(number);
    int i;

    // Return error code if driver not initialised
    if (!SevenSeg_isInitialised()) return SEVENSEG_ERRORNOINIT;

    // int *digits = convertIntToArrayOfDigits(number);
    // Store the digits in the number in `digits`
    getDigitsInNumber(number, digits);

    // Write every digit (or whitespace character) to the displays left-to-right
    for (i = 0; i < MAX_DISPLAYABLE_CHARACTERS; i++) {
        SevenSeg_displayDigit(i, *(digits + i));
    }

    return SEVENSEG_SUCCESS;
}

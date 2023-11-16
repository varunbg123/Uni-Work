/*
 * SDCard Driver
 * ------------------------------
 * Description:
 * Provides Methods to read, write, create files
 * to the SD card.
 *
 * Company: University of Leeds
 * Author: Emmanuel Leo
 * Varun Gonsalves and Kaif Kutchwala
 *
 */

#include <stdbool.h>

#include "FatFS/ff.h"

#define SDCARD_SUCCESS 0
#define SDCARD_ERRORNOINIT 1

#define MAX_CHARACTERS_PER_LINE 0xFFFFFFF

/*
 * SDCARD_createFile
 * This function is to create a file.
 *
 * Input:
 *     filename:   Name of the file to create
 */
void SDCARD_createFile(char filename[]);

/*
 * SDCARD_writeToFile
 * This function will write to the file.
 * The contents of the file will be overwritten.
 *
 * Input:
 *    filename:     the name of the file to be written to
 *    text:         the text to be written to the file
 */
void SDCARD_writeToFile(char filename[], char text[]);

/*
 * Function used to Mount the SD Card to initiliase the file system
 */
void SDCARD_mount(void);

/*
 * SDCARD_readFile
 * This function will return the contents of the specified file
 * up to the specified length.
 *
 * Input:
 *    filename:     the name of the file to be read
 *    length:       the length of text to be read
 *    line:         Array to store the line in.
 */
void SDCARD_readLine(char filename[], unsigned int length, char line[]);

/*
 * SDCARD_readFile
 *
 * Function to read the entire file
 *
 * Inputs:
 * 		filename:		The name of the file to read
 * 		lines:			2D arrays to store the lines read from file
 *
 * 	Output:
 * 		num_lines_read:	The number of lines read from the file
 *
 */
unsigned int SDCARD_readFile(char filename[], char **lines);

/*
 * SDCARD_checkFileExists
 *
 * Function to check if a file exists
 *
 * Inputs:
 * 		filename:		The name of the file to check for
 *
 * 	Output:
 * 		boolean:	    true if file exists else false
 *
 */
bool SDCARD_checkFileExists(char filename[]);
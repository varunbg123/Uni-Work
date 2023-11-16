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

// #define __GNUC__

#include "SDCard.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FatFS/ff.h"

FATFS FatFs; // Instance of the variable to handle format of the file system to be used with limited resources such as ROM/RAM
FIL Fil; // Instance of the data structure for open file/directory information

// Function used to Mount the SD Card to initiliase the file system
void SDCARD_mount() {
    f_mount(&FatFs, "", 0);
}

// This function will create a file.
void SDCARD_createFile(char filename[]) {
    FRESULT fr;

    // create a file if it doesnt exist
    fr = f_open(&Fil, filename, FA_READ | FA_CREATE_ALWAYS);
    // close the file
    fr = f_close(&Fil);
}

// This function will write to the file.
void SDCARD_writeToFile(char filename[], char text[]) {
    FRESULT fr;

    // length is the number of characters in the text
    UINT length = strlen(text);
    // Pointer to the variable to return number of bytes written
    UINT bw;

    // open the file, if it doesnt exist create the file
    fr = f_open(&Fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    // If operation was succesful
    if (fr == FR_OK) {
        // write the text to the file
        f_write(&Fil, text, length, &bw);
    }

    // close the file regardless
    fr = f_close(&Fil);
}

// This function will return the contents of the specified file
//  up to the specified length.
void SDCARD_readLine(char filename[], unsigned int length, char line[]) {
    FRESULT fr;

    // Open file in read mode
    fr = f_open(&Fil, filename, FA_READ);
    // if file is found
    if (fr == FR_OK) {
        // read line and write to 'line'
        f_gets(line, length + 1, &Fil);
    }
    // close the file
    fr = f_close(&Fil);
}

// Function to read the entire file
unsigned int SDCARD_readFile(char filename[], char **lines) {
    FRESULT fr;

    // initialise number of lines read to 0
    int num_lines_read = 0;
    // open the file in read mode
    fr = f_open(&Fil, filename, FA_READ);
    // if file found
    if (fr == FR_OK) {
        // read line until f_gets returns null
        // reading also writes the line contents to lines[num_lines_read]
        while (f_gets(lines[num_lines_read], MAX_CHARACTERS_PER_LINE, &Fil)) {
            // if line exists increment number of lines read by 1
            num_lines_read++;
        }
    }
    // close the file
    fr = f_close(&Fil);

    return num_lines_read;
}

// Function to check if file exists
bool SDCARD_checkFileExists(char filename[]) {
    FRESULT fr;
    FILINFO fno;

    fr = f_stat(filename, &fno);

    if (fr == FR_OK) {
        return true;
    } else {
        return false;
    }
}

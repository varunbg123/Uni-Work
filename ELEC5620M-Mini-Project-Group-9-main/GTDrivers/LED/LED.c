/*
 * LED Driver
 * ------------------------------
 * Description:
 * Provides Methods to control the LEDs
 * By writing values to the LED and also by setting a range 
 * to the LED
 * 
 * Company: University of Leeds
 * Author: Varun Gonsalves
 * Kaif Kutchwala and Emmanuel Leo
 *
 */


#include "LED.h"
#include <stdio.h>

//Driver Base Addresses
volatile unsigned int   *led_base_ptr    = 0x0;  //0xFF200000


//Driver Initialised
bool led_initialised = false;


//Each LED mapped to index
unsigned int LED_mappings[11] = {
    0x000,  // 00 0000 0000 -> 0
    0x200,  // 10 0000 0000 -> 1
    0x300,  // 11 0000 0000 -> 2
    0x380,  // 11 1000 0000 -> 3
    0x3C0,  // 11 1100 0000 -> 4
    0x3E0,  // 11 1110 0000 -> 5
    0x3F0,  // 11 1111 0000 -> 6
    0x3F8,  // 11 1111 1000 -> 7
    0x3FC,  // 11 1111 1100 -> 8
    0x3FE,  // 11 1111 1110 -> 9
    0x3FF,  // 11 1111 1111 -> 10
};

//Function to initialise the Servo controller
signed int LED_initialise( unsigned int base_address ){
    //Set the local base address pointer
    led_base_ptr = (unsigned int *) base_address;
    // Set bool as initialised
    led_initialised = true;
    
    return LED_SUCCESS;
}

//Check if driver initialised
bool LED_isInitialised() {
    return led_initialised;
}

//Write value to the LED pointer
signed int LED_write(unsigned int value){
    if (!LED_isInitialised()) return LED_ERRORNOINIT;

    *led_base_ptr = value;

    return LED_SUCCESS;
}

//Use all 10 LEDs to show value in the min-max range
//The LEDs will light from left to right 
//The number of LEDs that light up is proportional to the value
//in the min-max range
unsigned int LED_setValueInRange(float min, float max, float value){
	int led_value;
	int led_register_value;

    if (!LED_isInitialised()) return LED_ERRORNOINIT;

    //If value is above maximum, set equal to maximum
    if(value > max)
        value = max;

    //Convert value from min-max range to 0-10 range    
    led_value = (value/(max-min))*10;
    //Get the LED register value for the display value
    led_register_value = LED_mappings[led_value];
    // Write the register value to the pointer
    LED_write(led_register_value);

    return LED_SUCCESS;
}

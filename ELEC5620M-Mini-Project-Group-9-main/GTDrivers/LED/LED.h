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

#include <stdbool.h> 

#define LED_SUCCESS       0
#define LED_ERRORNOINIT   1

/*
 * LED_initialise
 * Set the base address for the LEDs
*/
signed int LED_initialise( unsigned int base_address );

/*
 * LED_isInitialised
 * A check to make sure the LEDs are initialised
*/
bool LED_isInitialised( void );

/*
 * LED_write
 * This function is to write values to the LEDs pointer
 * 
 * Input:
 *     Value:   This value is written to the LED pointer
*/
signed int LED_write(unsigned int value);

/*
 * LED_setValueInRange
 * This function will use all 10 LEDs to diplay a value in the 
 * min-max range. It converts the value to a range of 1-10. Which is then
 * written to the LED register
 * 
 * Input:
 *    min:     minimum value
 *    max:     maximum value
 *    value:   value in the min max range
*/
unsigned int LED_setValueInRange(float min, float max, float value);

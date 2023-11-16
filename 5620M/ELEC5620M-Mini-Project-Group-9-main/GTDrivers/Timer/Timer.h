/**
 * Timer.h
 *
 * Timer functions are available to call from this file
 *
 * Varun Gonsalves, Emmanuel Leo, Kaif Kutchwala
 *
 * Date: 06/05/2023
 */

#include <stdbool.h>

#define TIMER_SUCCESS 0
#define TIMER_ERRORNOINIT 1

/**
 * Timer_initialise
 *
 * Function to initialise the Timer
 *
 * Inputs:
 * 		base_address: 	connecting to the correct base address for the device
 *
 **/
signed int Timer_initialise(unsigned int base_address);

/**
 * Timer_isInitialised
 *
 * Check if driver initialised
 *
 **/
bool Timer_isInitialised(void);

/**
 * Timer_setLoad
 *
 * Setting the timer load value into the register
 *
 * Inputs:
 * 		load_value: 	load value to set timer
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setLoad(unsigned int load_value);

/**
 * Timer_getValue
 *
 * Getting the current timer value
 *
 * Outputs:
 * 		timer_base_ptr[TIMER_VALUE]:	value from the pointer
 **/
unsigned int Timer_getValue(void);

/**
 * Timer_setPrescalar
 *
 * Setting the Prescalar Value into the control register
 * The prescalar will scale the load value by (input + 1).
 *
 * Input:
 * 		preScalar_value:	input value for the prescalar
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setPrescalar(unsigned int prescalar_value);

/**
 * Timer_setEnable
 *
 * Setting the Enable Value into the control register.
 * This will enable the timer
 *
 * Input:
 * 		enable_value:	input value for the enable
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setEnable(unsigned int enable_value);

/**
 * Timer_setAutomaticReload
 *
 * Setting the Automatic Reload Value into the control register
 * This will reset the value once the the timer has finished counting down
 *
 * Input:
 * 		automatic_reloadValue:	input value for the automatic reload
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setAutomaticReload(unsigned int automatic_reload_value);

/**
 * Timer_setISRValue
 *
 * Setting the ISR Value into the control register
 *
 * Input:
 * 		ISR_value:	input value for the ISR
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setISRValue(unsigned int ISR_value);

/**
 * Timer_setControl
 *
 * Setting the whole control register in one function. This includes the :
 * Prescalar, ISR, Overflow and the Enable Timer
 *
 * Input:
 * 		prescalar_value:		input value for the prescalar
 * 		ISR :				input value for the ISR
 * 		overflow:	input value for the automatic reload
 * 		enable_timer:			input value for the enable
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setControl(unsigned int prescalar_value, unsigned int ISR, unsigned int overflow, unsigned int enable_timer);

/**
 * Timer_getInterruptStatus
 *
 * Getting the interrupt status
 **/
unsigned int Timer_getInterruptStatus(void);

/**
 * Timer_resetInterrupt
 *
 * Reseting the interrupt by setting the interrupt to 1 to clear the flag
 *
 **/
unsigned int Timer_resetInterrupt(void);

/**
 * Timer_setPeriod
 *
 * Setting the Timer period. The input for this will be in milliseconds.
 * This will set the prescalar, isr, automatic reload and the enable automatically from the input
 *
 * Input:
 * 		time:	input value for the time in ms
 *
 * Outputs:
 * 		TIMER_SUCCESS:	flag check to make sure the function worked
 **/
unsigned int Timer_setPeriod(unsigned int time);

/**
 * Timer_getValueMS
 *
 * Getting the current timer value but in milliseconds.
 * This can be used for multiple clock and be used in ms. However when
 * using mutiple clock make sure to set the update period as a multiple of the Period
 *
 * Outputs:
 * 		time_ms:	current time from the pointer in milliseconds
 **/
unsigned int Timer_getValueMS(void);

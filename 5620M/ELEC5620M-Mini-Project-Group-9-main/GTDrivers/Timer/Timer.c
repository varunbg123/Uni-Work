/**
 * Timer.C
 *
 * Timer functions are available to call from this file
 *
 * Varun Gonsalves, Emmanuel Leo, Kaif Kutchwala
 *
 * Date: 06/05/2023
 */

#include "Timer.h"

#include <stdio.h>

#include "HPS_Watchdog/HPS_Watchdog.h"

// Driver Base Addresses
volatile unsigned int *timer_base_ptr = 0x0;  // 0xFFFEC600

// Driver Initialised
bool timer_initialised = false;

// Register Offsets
#define TIMER_LOAD (0x00 / sizeof(unsigned int))
#define TIMER_VALUE (0x04 / sizeof(unsigned int))
#define TIMER_CONTROL (0x08 / sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C / sizeof(unsigned int))

// Clock frequency at 1/4 of the 900MHz Clock
volatile float timer_frequency = 225000000.0;

// Function to initialise the Timer
signed int Timer_initialise(unsigned int base_address) {
    // Initialise base address pointers
    timer_base_ptr = (unsigned int *)base_address;
    // Ensure timer initialises to disabled
    timer_base_ptr[TIMER_CONTROL] = 0;
    // Timer now initialised
    timer_initialised = true;

    return TIMER_SUCCESS;
}

// Check if driver initialised
bool Timer_isInitialised() {
    return timer_initialised;
}

// Setting the timer load value into the register
unsigned int Timer_setLoad(unsigned int load_value) {
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // Ensure timer initialises to disabled
    timer_base_ptr[TIMER_LOAD] = load_value;

    return TIMER_SUCCESS;
}

// Getting the current timer value
unsigned int Timer_getValue(void) {
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    return timer_base_ptr[TIMER_VALUE];
}

// Setting the Prescalar Value into the control register
unsigned int Timer_setPrescalar(unsigned int prescalar_value) {
    unsigned int timer_control_value;
    unsigned int mask;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    timer_control_value = timer_base_ptr[TIMER_CONTROL];
    // 1111,1111,1111,1111,0000,0000,1111,1111
    mask = 0xFFFF00FF;
    // Keep the values from original timer control register but clears the prescalar values
    timer_control_value &= mask;
    // add in the values of the prescalar
    timer_control_value |= (prescalar_value << 8);
    // write to the timer control address
    timer_base_ptr[TIMER_CONTROL] = timer_control_value;

    return TIMER_SUCCESS;
};

// Setting the Enable Value into the control register
unsigned int Timer_setEnable(unsigned int enable_value) {
    unsigned int timer_control_value;
    unsigned int mask;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    timer_control_value = timer_base_ptr[TIMER_CONTROL];
    // 1111,1111,1111,1111,1111,1111,1111,1110
    mask = 0xFFFFFFFE;
    // Keep the values from original timer control but clears the enable value
    timer_control_value &= mask;
    // add in the values of the Enable Value
    timer_control_value |= (enable_value << 0);
    // write to the timer control address
    timer_base_ptr[TIMER_CONTROL] = timer_control_value;

    return TIMER_SUCCESS;
};

// Setting the Automatic Reload Value into the control register
unsigned int Timer_setAutomaticReload(unsigned int automatic_reload_value) {
	unsigned int timer_control_value;
	unsigned int mask;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    timer_control_value = timer_base_ptr[TIMER_CONTROL];
    // 1111,1111,1111,1111,1111,1111,1111,1101
    mask = 0xFFFFFFFD;
    // Keep the values from original timer control but clears the Automatic reload Value
    timer_control_value &= mask;
    // add in the values of the Automatic Reload Value
    timer_control_value |= (automatic_reload_value << 1);
    // write to the timer control address
    timer_base_ptr[TIMER_CONTROL] = timer_control_value;

    return TIMER_SUCCESS;
};

// Setting the ISR Value into the control register
unsigned int Timer_setISRValue(unsigned int ISR_value) {
	unsigned int timer_control_value;
	unsigned int mask;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    timer_control_value = timer_base_ptr[TIMER_CONTROL];
    // 1111,1111,1111,1111,1111,1111,1111,1011
    mask = 0xFFFFFFFB;
    // Keep the values from original but clears the ISR value
    timer_control_value &= mask;
    // add in the values of the ISR Value
    timer_control_value |= (ISR_value << 2);
    // write to the timer control address
    timer_base_ptr[TIMER_CONTROL] = timer_control_value;

    return TIMER_SUCCESS;
};

// Setting the complete control registers
unsigned int Timer_setControl(unsigned int prescalar_value, unsigned int ISR, unsigned int overflow, unsigned int enable_timer) {
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // Set the "Prescalar" value, Enable the timer (E = 1), Set Automatic reload on overflow (A = 1), and disable ISR (I = 0)
    timer_base_ptr[TIMER_CONTROL] = (prescalar_value << 8) | (ISR << 2) | (overflow << 1) | (enable_timer << 0);

    return TIMER_SUCCESS;
}

// Getting the interrupt status
unsigned int Timer_getInterruptStatus(void) {
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    return timer_base_ptr[TIMER_INTERRUPT];
}

// Reseting the interrupt
unsigned int Timer_resetInterrupt(void) {
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    if (timer_base_ptr[TIMER_INTERRUPT] & 0x1) {
        // If the timer interrupt flag is set, clear the flag
        timer_base_ptr[TIMER_INTERRUPT] = 0x1;
    }
    return TIMER_SUCCESS;
}

// Set a Time period in ms
unsigned int Timer_setPeriod(unsigned int time) {
	float prescalar_value;
	float load_value;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    prescalar_value = ((time / 1000) / 18);
    // write the prescalar value to the register
    // Timer_setPrescalar(preScalarValue);
    Timer_setControl(prescalar_value, 0, 1, 1);
    // this equation will get the load value to put into the load register
    // Using the prescalar value, the timer frequency and the timer period a load value
    // can be calculated
    load_value = (time / (1000 * (prescalar_value + 1))) * (timer_frequency);
    // unsigned int loadvalue = midvalue/(preScalarValue + 1);
    //  Set the load value into the timer
    Timer_setLoad(load_value);

    return TIMER_SUCCESS;
};

// Getting the current timer value in ms
unsigned int Timer_getValueMS(void) {
	unsigned int timer_control_value;
	unsigned int mask;
	float prescalar_value;
	float timer_value;
	float time_ms;
    // check if timer has initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // get the prescalar value
    timer_control_value = timer_base_ptr[TIMER_CONTROL];
    // 0000,0000,0000,0000,1111,1111,0000,0000
    mask = 0x0000FF00;
    // Clears all the values except for the prescalar bits
    timer_control_value &= mask;
    // Shift to obtain the prescalar value
    // int prescalar_value = (timer_control_value >> 8);
    prescalar_value = (timer_control_value >> 8);
    // get the value from the register
    timer_value = timer_base_ptr[TIMER_VALUE];
    // convert the value to ms
    time_ms = (timer_value / timer_frequency) * 1000 * (prescalar_value + 1);

    return time_ms;
}

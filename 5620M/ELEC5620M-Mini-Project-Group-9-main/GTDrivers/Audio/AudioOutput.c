/*
 *  DE1-SoC Audio Output Driver
 * ------------------------------
 * Description:
 * Light wrapper around WM8731 driver that provides methods
 * to play pre defined tones and frequencies.
 *
 * Company: University of Leeds
 * Author: T Carpenter, Kaif Kutchwala, Emmanuel Leo, Varun Gonsalves
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 03/05/2023 | Creation of driver
 */

// Include External Libraries
#include "AudioOutput.h"  //Include header for the Audio Output Driver

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "DE1SoC_WM8731/DE1SoC_WM8731.h"  // Include Codec for the WM8731 peripheral
#include "HPS_Watchdog/HPS_Watchdog.h"    //Include Driver of the HPS Watchdog

// Global Variables
// Define Pointers

// Debugging - display FIFO space on red LEDs. UNCOMMENT THE NEXT LINE TO RUN FiFO SPACE DEBUG.
// #define WITH_DEBUGGING
#ifdef WITH_DEBUGGING
volatile unsigned int *LEDR = (unsigned int *)0xFF200000;  // Red LEDs base address
#endif

volatile unsigned char *fifospace_ptr;
volatile unsigned int *audio_left_ptr;
volatile unsigned int *audio_right_ptr;

double phase = 0.0;  // Phase accumulator

// Function that takes in a certain frequency and processes it to fulfill a single iteration of generating the desired output to be sent to the desired channel(s)
// NOTE: Ensure that the function is within a loop so it can generate the whole waveform to be heard
int AUDIOOUTPUT_playTone(double frequency, double volume, unsigned int channel) {
    // Initialise Phase Accumulator
    double inc = frequency * PI2 / F_SAMPLE;  // Calculate the phase increment based on desired frequency
    double ampl = 8388608.0 * volume / 100;   // Calculate the desired amplitude. WARNING: DEAFENING IF TOO HIGH!
    signed int audio_sample = 0;              // Variable to store the sample to be output to the desired channel(s)

    /// Grab the FIFO Space and Audio Channel Pointers
    fifospace_ptr = WM8731_getFIFOSpacePtr();
    audio_left_ptr = WM8731_getLeftFIFOPtr();
    audio_right_ptr = WM8731_getRightFIFOPtr();

    // Check the FIFO space before writing/reading values to the pointers of the left/right channels
    if ((fifospace_ptr[2] > 0) && (fifospace_ptr[3] > 0)) {
        // Increment the phase
        phase = phase + inc;
        // Ensure phase is wrapped to range 0 to 2*Pi (range of sin function)
        while (phase >= PI2) {
            phase = phase - PI2;
        }
        // Calculate next sample of the output tone.
        audio_sample = (signed int)(ampl * sin(phase));
        // Output tone to left and right channels.
        AUDIOOUTPUT_writeToChannel(channel, audio_sample, audio_sample);

// Debugging - display FIFO space on red LEDs.
#ifdef WITH_DEBUGGING
        *LEDR = fifospace_ptr[2];  // Output 'WSRC' register to the red LEDs
#endif
                                   // Clear both FIFOs
        WM8731_clearFIFO(true, true);  // Clear buffer on each iteration so it never overflows
        // Finally reset the watchdog.
        HPS_ResetWatchdog();
    }
    return AUDIOOUTPUT_SUCCESS;
}

void AUDIOOUTPUT_writeToChannel(unsigned int channel_choice, signed int left_value, signed int right_value) {
    switch (channel_choice) {
        case AUDIO_BOTHCHANNELS:  // Output to both the left and right channels
            *audio_left_ptr = left_value;
            *audio_right_ptr = right_value;
            break;
        case AUDIO_LEFTCHANNEL:  // Output to the right channel only
            *audio_left_ptr = left_value;
            break;
        case AUDIO_RIGHTCHANNEL:  // Output to the left channel only
            *audio_right_ptr = right_value;
            break;
    }
}

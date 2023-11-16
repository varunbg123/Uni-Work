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

#ifndef AUDIO_OUTPUT_
#define AUDIO_OUTPUT_

// Frequency Definition of standard notes
#define C0 16.35
#define C_SHARP_0 16.35
#define D_FLAT_0 17.32
#define D0 18.35
#define D_SHARP_0 18.35
#define E_FLAT_0 19.45
#define E0 20.60
#define F0 21.83
#define F_SHARP_0 21.83
#define G_FLAT_0 23.12
#define G0 24.50
#define G_SHARP_0 24.50
#define A_FLAT_0 25.96
#define A0 27.50
#define A_SHARP_0 27.50
#define B_FLAT_0 29.14
#define B0 30.87
#define C1 32.70
#define C_SHARP_1 32.70
#define D_FLAT_1 34.65
#define D1 36.71
#define D_SHARP_1 36.71
#define E_FLAT_1 38.89
#define E1 41.20
#define F1 43.65
#define F_SHARP_1 43.65
#define G_FLAT_1 46.25
#define G1 49.00
#define G_SHARP_1 49.00
#define A_FLAT_1 51.91
#define A1 55.00
#define A_SHARP_1 55.00
#define B_FLAT_1 58.27
#define B1 61.74
#define C2 65.41
#define C_SHARP_2 65.41
#define D_FLAT_2 69.30
#define D2 73.42
#define D_SHARP_2 73.42
#define E_FLAT_2 77.78
#define E2 82.41
#define F2 87.31
#define F_SHARP_2 87.31
#define G_FLAT_2 92.50
#define G2 98.00
#define G_SHARP_2 98.00
#define A_FLAT_2 103.83
#define A2 110.00
#define A_SHARP_2 110.00
#define B_FLAT_2 116.54
#define B2 123.47
#define C3 130.81
#define C_SHARP_3 130.81
#define D_FLAT_3 138.59
#define D3 146.83
#define D_SHARP_3 146.83
#define E_FLAT_3 155.56
#define E3 164.81
#define F3 174.61
#define F_SHARP_3 174.61
#define G_FLAT_3 185.00
#define G3 196.00
#define G_SHARP_3 196.00
#define A_FLAT_3 207.65
#define A3 220.00
#define A_SHARP_3 220.00
#define B_FLAT_3 233.08
#define B3 246.94
#define C4 261.63
#define C_SHARP_4 261.63
#define D_FLAT_4 277.18
#define D4 293.66
#define D_SHARP_4 293.66
#define E_FLAT_4 311.13
#define E4 329.63
#define F4 349.23
#define F_SHARP_4 349.23
#define G_FLAT_4 369.99
#define G4 392.00
#define G_SHARP_4 392.00
#define A_FLAT_4 415.30
#define A4 440.00
#define A_SHARP_4 440.00
#define B_FLAT_4 466.16
#define B4 493.88
#define C5 523.25
#define C_SHARP_5 523.25
#define D_FLAT_5 554.37
#define D5 587.33
#define D_SHARP_5 587.33
#define E_FLAT_5 622.25
#define E5 659.25
#define F5 698.46
#define F_SHARP_5 698.46
#define G_FLAT_5 739.99
#define G5 783.99
#define G_SHARP_5 783.99
#define A_FLAT_5 830.61
#define A5 880.00
#define A_SHARP_5 880.00
#define B_FLAT_5 932.33
#define B5 987.77
#define C6 1046.50
#define C_SHARP_6 1046.50
#define D_FLAT_6 1108.73
#define D6 1174.66
#define D_SHARP_6 1174.66
#define E_FLAT_6 1244.51
#define E6 1318.51
#define F6 1396.91
#define F_SHARP_6 1396.91
#define G_FLAT_6 1479.98
#define G6 1567.98
#define G_SHARP_6 1567.98
#define A_FLAT_6 1661.22
#define A6 1760.00
#define A_SHARP_6 1760.00
#define B_FLAT_6 1864.66
#define B6 1975.53
#define C7 2093.00
#define C_SHARP_7 2093.00
#define D_FLAT_7 2217.46
#define D7 2349.32
#define D_SHARP_7 2349.32
#define E_FLAT_7 2489.02
#define E7 2637.02
#define F7 2793.83
#define F_SHARP_7 2793.83
#define G_FLAT_7 2959.96
#define G7 3135.96
#define G_SHARP_7 3135.96
#define A_FLAT_7 3322.44
#define A7 3520.00
#define A_SHARP_7 3520.00
#define B_FLAT_7 3729.31
#define B7 3951.07
#define C8 4186.01
#define C_SHARP_8 4186.01
#define D_FLAT_8 4434.92
#define D8 4698.63
#define D_SHARP_8 4698.63
#define E_FLAT_8 4978.03
#define E8 5274.04
#define F8 5587.65
#define F_SHARP_8 5587.65
#define G_FLAT_8 5919.91
#define G8 6271.93
#define G_SHARP_8 6271.93
#define A_FLAT_8 6644.88
#define A8 7040.00
#define A_SHARP_8 7040.00
#define B_FLAT_8 7458.62
#define B8 7902.13

// Define some useful constants
#define F_SAMPLE 48000.0   // Sampling rate of WM8731 Codec (Do not change)
#define PI2 6.28318530718  // 2 x Pi      (Apple or Peach?)

#define AUDIO_BOTHCHANNELS 0  // Channel Selection Option for writing to both channels
#define AUDIO_RIGHTCHANNEL 1  // Channel Selection Option for writing to the right channel
#define AUDIO_LEFTCHANNEL 2   // Channel Selection Option for writing to the left channel

// Define Status codes
#define AUDIOOUTPUT_SUCCESS 1  // Value to be returned upon the successful completion of a function/process

// Define Function Prototypes

/*
 *  AUDIOOUTPUT_playTone
 *
 *  This function takes in the desired frequency, processes it, and
 *  outputs it to the specified channel(s)
 *
 *  Inputs:
 *              frequency:              Frequency to be played (continuous positive decimal values)
 *              volume:                 Volume of the sample to be sent to the channel (0 - 100)
 *              channel:                Channel the desired sample is to be played in
 *                                      0 - Both Channels, 1 - Left Channel, 2 - Right Channel
 *
 *  Output:
 *              AUDIOOUT_SUCCESS:             Signed int that symbolises whether sample was played successfully or not (1)
 */
signed int AUDIOOUTPUT_playTone(double frequency, double volume, unsigned int channel);

/*
 *   AUDIOOUTPUT_writeToChannel
 *
 *   This Function writes a sample value to the desired channl(s)
 *
 *   Inputs:
 *               channel_choice:        The desired FIFO to write to and consequently the desired Channel to ouput on (0 - 3)
 *               left_value:            Value to write to the left FIFO pointer (continuous positive decimal values)
 *               right_value:           Value to write to the right FIFO pointer (continuous positive decimal values)
 *
 */
void AUDIOOUTPUT_writeToChannel(unsigned int channel_choice, signed int left_vlaue, signed int right_value);

#endif

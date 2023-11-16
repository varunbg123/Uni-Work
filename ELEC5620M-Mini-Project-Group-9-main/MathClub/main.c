/*
 * main.c
 *
 *  Created on: 14 Apr 2023
 *      Author: Varun Gonsalves, Kaif Kutchwala, Emmanuel Leo
 */

#include <stdio.h>
#include <stdlib.h>

#include "Audio/AudioOutput.h"
#include "DE1SoC_WM8731/DE1SoC_WM8731.h"
#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "LCD/LCD.h"
#include "LED/LED.h"
#include "QuestionGenerator/QuestionGenerator.h"
#include "SDCard/SDCard.h"
#include "Servo/DE1SOC_Servo.h"
#include "SevenSeg/SevenSeg.h"
#include "Timer/Timer.h"

volatile unsigned int *key_ptr = (unsigned int *)0xFF200050;     // KEYS 0-3 (push buttons)
volatile unsigned int *switch_ptr = (unsigned int *)0xFF200040;  // SWITCHES 0-10

// Store the round time limit to reset the remaining time
// correctly based on difficulty.
float round_time_limit;

// Store the remaining time in the round to display to the user
float round_time_remaining;

// Store the last time the round time was update to ensure clock
// goes down by 1 second
unsigned int last_round_time_update;

// Store the state of keys to determine which one is clicked
unsigned int keys_pressed;

// Store the state of the keys last time we checked.
// This allows us to determine when a key is pressed, then released.
unsigned int key_last_state = 0;

/**
 * getPressedKeys
 *
 * Helper function to determine which keys have been pressed
 * and then released later.
 *
 * Arguments:
 * 		key_last_state: 	4-value array of the last state of each key.
 *
 * Returns: Mask, 1 for keys which have been pressed, 0 for keys not.
 */
unsigned int getPressedKeys() {
    // Store the current state of the keys.
    unsigned int key_current_state = *key_ptr;

    // If the key was down last cycle, and is up now, mark as pressed.
    unsigned int keys_pressed = (~key_current_state) & (key_last_state);

    // Save the key state for next time, so we can compare the next state to this.
    key_last_state = key_current_state;

    // Return result.
    return keys_pressed;
}

void exitOnFail(signed int status, signed int successStatus) {
    if (status != successStatus) {
        exit((int)status);  // Add breakpoint here to catch failure
    }
}

// Resets the round time to the time limit
void resetRoundTime() {
    round_time_remaining = round_time_limit;
}

// Resets the level, score and round time for the game
void resetGameProgress() {
    GameEngine_setLevel(0);  // reset the score
    GameEngine_setScore(0);  // reset the level
    resetRoundTime();        // reset the round time
}

// Main Function
// =============
int main(void) {
    // Initialise the LCD Display.
    exitOnFail(
        LCD_initialise(0xFF200060, 0xFF200080),  // Initialise LCD
        LCD_SUCCESS);                            // Exit if not successful

    // Initialise the timer
    exitOnFail(
        Timer_initialise(0xFFFEC600),  // Initialise Timer Controller
        TIMER_SUCCESS);                // Exit if not successful

    // set timer period
    exitOnFail(
        Timer_setPeriod(60000),  // Set Timer Period Timer Controller
        TIMER_SUCCESS);          // Exit if not successful

    // Initialise the servo
    exitOnFail(
        Servo_initialise(0xFF2000C0),  // Initialise Servo Controller
        SERVO_SUCCESS);                // Exit if not successful
    exitOnFail(
        Servo_enable(0, true),  // Enable Servo 0.
        SERVO_SUCCESS);         // Exit if not successful
    exitOnFail(
        Servo_pulseWidthRange(0, true),  // Double pulse width.
        SERVO_SUCCESS);                  // Exit if not successful

    // Initialise the LEDs
    exitOnFail(
        LED_initialise(0xFF200000),  // Initialise LED
        LED_SUCCESS);                // Exit if not successful

    // Initialise the Seven Segment Displays
    exitOnFail(
        SevenSeg_initialise(0xFF200020, 0xFF200030),
        SEVENSEG_SUCCESS);

    // Initialise the Audio / WM8731
    exitOnFail(
        WM8731_initialise(0xFF203040),
        WM8731_SUCCESS);

    // Mount the SD Card
    SDCARD_mount();
    // Initialise game engine state variables
    GameEngine_initialise("mathclub.txt");
    // Get the initial round time limit
    round_time_limit = GameEngine_getTimeLimit();
    // Reset round time to be maximum i.e. equal to the time limit
    resetRoundTime();

    // Corresponding value set to 1 when a key is pressed then released.
    // Set initial value to no keys pressed
    keys_pressed = 0;

    while (1) {
        // Get the current state of the game
        int state = GameEngine_getState();
        // Get the current timer value in milliseconds
        unsigned int current_time = Timer_getValueMS();
        // Get the current round time limit for the game
        round_time_limit = GameEngine_getTimeLimit();

        // Check if any buttons have been pressed and released.
        // The keys_pressed variable will need to be bit masked to determine if a
        // specific key was pressed.
        keys_pressed = getPressedKeys();

        if (state == GAMEENGINE_MAINMENU) {
            // If game is on main menu, display the main menu
            GameEngine_displayMainMenu();

            // Handle button presses
            if (keys_pressed & 0x1) {
                // if player clicks Btn 0 i.e. selects Play option
                resetGameProgress();                      // reset the level, score and round time
                GameEngine_setState(GAMEENGINE_PLAYING);  // set game state to PLAYING
            }
            if (keys_pressed & 0x2) {
                // if player clicks Btn 1 i.e. selects increase volume option
                GameEngine_increaseVolume();  // increase the volume
            }
            if (keys_pressed & 0x4) {
                // if player clicks Btn 2 i.e. selects decrease volume option
                GameEngine_decreaseVolume();  // decrease the volume
            }

            // If SW0 is on
            if (*switch_ptr & 0x1) {
                GameEngine_setGameMode(GAMEENGINE_HARD);  // set the game mode to HARD
            } else {
                GameEngine_setGameMode(GAMEENGINE_EASY);  // else set the game mode to EASY
            }

        } else if (state == GAMEENGINE_PLAYING) {
            // If game is in PLAYING state
            // Display the current level with the remaining round time
            GameEngine_displayLevel(round_time_remaining);

            // If player runs out of time, end game.
            if (round_time_remaining <= 0) {
                resetGameProgress();  // reset the level, score and round time

                // set game over animation last update time to current time
                // this is so that the animation runs for the required duration
                GameEngine_setGameOverLastUpdateTime(current_time);

                GameEngine_setState(GAMEENGINE_GAMEOVER);  // set game state to GAMEOVER
            }

            // If any option is selected i.e. any button is clicked.
            if (keys_pressed & 0xF) {
                resetRoundTime();  // reset the round time
            }

            // Handle button presses
            if (keys_pressed & 0x1) {
                // if player clicks Btn 0 i.e. selects option 1
                GameEngine_enterOption(GAMEENGINE_OPTION1);  // enter option 1
            }
            if (keys_pressed & 0x2) {
                // if player clicks Btn 1 i.e. selects option 2
                GameEngine_enterOption(GAMEENGINE_OPTION2);  // enter option 2
            }
            if (keys_pressed & 0x4) {
                // if player clicks Btn 2 i.e. selects option 3
                GameEngine_enterOption(GAMEENGINE_OPTION3);  // enter option 3
            }
            if (keys_pressed & 0x8) {
                // if player clicks Btn 3 i.e. selects option 4
                GameEngine_enterOption(GAMEENGINE_OPTION4);  // enter option 4
            }

            // If SW0 is on 9 is on set game mode to paused
            if (*switch_ptr & 0x200) {
                GameEngine_setState(GAMEENGINE_PAUSED);
            }

        } else if (state == GAMEENGINE_PAUSED) {
            // If game is paused, display pause menu
            GameEngine_displayPauseMenu();

            // Handle button presses
            if (keys_pressed & 0x1) {
                // if player clicks Btn 0 i.e. selects exit option
                resetGameProgress();                       // reset the level, score and round time
                GameEngine_setState(GAMEENGINE_MAINMENU);  // exit to main menu
            }
            if (keys_pressed & 0x2) {
                // if player clicks Btn 1 i.e. selects increase volume option
                GameEngine_increaseVolume();  // increase the volume
            }
            if (keys_pressed & 0x4) {
                // if player clicks Btn 2 i.e. selects decrease volume option
                GameEngine_decreaseVolume();  // decrease the volume
            }

            // If SW0 is off 9 is on set game mode to PLAYING
            if (!(*switch_ptr & 0x200)) {
                GameEngine_setState(GAMEENGINE_PLAYING);
            }

        } else if (state == GAMEENGINE_LEVELUP) {
            // If state is LEVELUP display level up animation
            GameEngine_levelUp();
        } else if (state == GAMEENGINE_WIN) {
            // If state is WIN display celebrate up animation
            GameEngine_celebrate();
        } else if (state == GAMEENGINE_GAMEOVER) {
            // If state is GAMEOVER display game over up animation
            GameEngine_gameOver();
        } else {
            // If game is in invalid state, reset state and send to main menu
            resetGameProgress();
            resetRoundTime();
            GameEngine_setState(GAMEENGINE_MAINMENU);
        }

        // Update the remaining time in the round
        // First check if game is in PLAYING state
        // This is to ensure time is not running down when paused or viewing animations
        if (state == GAMEENGINE_PLAYING) {
            // Check if one second has passed since the last time remaining time was
            // updated
            if ((last_round_time_update - current_time) >= 1000.0) {
                round_time_remaining--;  // reduce the time by 1 unit

                // round time cannot be below 0
                if (round_time_remaining < 0) {
                    round_time_remaining = 0.0;
                }

                // update the last_round_time_update to current time
                last_round_time_update = current_time;
            }
        }

        // Refresh the screen to show new contents
        GraphicsEngine_update();

        // Next, make sure we clear the private timer interrupt flag if it is set
        if (Timer_getInterruptStatus() & 0x1) {
            // If the timer interrupt flag is set, clear the flag
            Timer_resetInterrupt();
        }

        // Finally, reset the watchdog timer.
        HPS_ResetWatchdog();
    }
}

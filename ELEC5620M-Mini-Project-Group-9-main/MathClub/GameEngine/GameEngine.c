/**
 * Game Engine.c
 *
 * Implementation of Game engine functions in this file
 *
 * Varun Gonsalves, Emmanuel Leo, Kaif Kutchwala
 *
 * Date: 06/05/2023
 */

#include "GameEngine.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../QuestionGenerator/QuestionGenerator.h"
#include "Audio/AudioOutput.h"
#include "LED/LED.h"
#include "SDCard/SDCard.h"
#include "Servo/DE1SoC_Servo.h"
#include "SevenSeg/SevenSeg.h"
#include "Timer/Timer.h"

// indexes for actions in last_update_time, timer_update_period,
// and last_update_values arrays
int gameover_led_show = 0;
int levelup_led_show = 1;
int celebrate_led_show = 2;
int celebrate_text_display = 3;
int levelup_text_display = 4;
int gameover_text_display = 5;
int levelup_audio = 6;
int celebrate_audio = 7;
int gameover_audio = 8;
int levelup = 9;
int celebrate = 10;
int gameover = 11;

// last update time for each action
unsigned int last_update_time[12] = {0};

// Setting the update period for each action
unsigned int timer_update_period[12] = {1000, 100, 100, 400, 100, 100, 400, 400, 400, 3000, 5000, 5000};

// this is to set the led display initially then they will be updated to this array
unsigned int last_update_values[12] = {0x000, 0x011, 0x015, 0, 0, 0, A4, C4, A3, 0, 0, 0};

// victory to be displayed when the game is completed
char* celebrate_text = "      victory     ";

// text to be displayed when answer is correct
char* level_up_text = "      correct ans     ";

// text to be displayed when player loses
char* game_over_text = "      try again     ";

// flags used to check when to play audio for actions
bool play_levelup_audio = true;
bool play_gameover_audio = true;
bool play_celebrate_audio = true;

// Game State Variables
unsigned int state = GAMEENGINE_MAINMENU;                                // Initial screen is Main Menu
unsigned int level = 0;                                                  // Start at level 0
unsigned int game_mode = GAMEENGINE_EASY;                                // Default game mode is Easy
unsigned int score = 0;                                                  // Initial score is 0
unsigned int high_score = 0;                                             // Initial high score is 0
float time_limit = 30.0;                                                 // Default time limit is 30 seconds
float volume = 5.0;                                                      // Default volume is 50%
QuestionResult current_question;                                         // Declare question
unsigned int easy_question_levels[10] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2};  // Level for every round on easy mode
unsigned int hard_question_levels[10] = {1, 1, 2, 2, 2, 2, 3, 3, 3, 3};  // Level for every round on hard mode
unsigned int timer_color[3] = {0, 255, 0};                               // Timer color initially is Green
char store_filename[];                                                   // = "mathclub.txt";

// TODO: make score a function of time taken to answer and difficulty.

// Helper methods
void displaySentence(char* text, int update_value_index) {
    // Get current time
    unsigned int current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[update_value_index] - current_time) >= timer_update_period[update_value_index]) {
        // shift the word being displayed by 1 character
        char display_word[6];
        strncpy(display_word, text + last_update_values[update_value_index], 6);
        // display word on seven segment display
        SevenSeg_displayWord(display_word);
        // increment current display value
        last_update_values[update_value_index]++;

        // If reached the end of the sentence, reset to start
        if (last_update_values[update_value_index] + 6 > strlen(text)) {
            last_update_values[update_value_index] = 0;
        }
        // Update last timer update value
        last_update_time[update_value_index] = current_time;
    }
}

// Plays a single note for specified action
// The audio effect plays two
// notes the first note plays for 1/3rd of the duration
// and the second plays for 2/3rds of the duration.
void playSoundEffectFor(unsigned int action_id, double note1, double note2) {
    unsigned int current_time, time_elapsed, note1_play_period;
    double tone;
    // Set the last update time to current time
    last_update_time[action_id] = Timer_getValueMS();
    // This is a blocking operation but audio sounds are played
    // for very short durations and do not affect the rest of the game
    // Non-blocking could not be done as the values were not being
    // written fast enough to the FIFO registers resulting in no output
    while (1) {
        // Get current timer value
        current_time = Timer_getValueMS();
        // If enough time has passed
        time_elapsed = last_update_time[action_id] - current_time;

        if (time_elapsed >= timer_update_period[action_id]) {
            // stop playing audio
            break;
        }
        // Play audio
        note1_play_period = timer_update_period[action_id] / 3;
        tone = time_elapsed > note1_play_period ? note2 : note1;
        AUDIOOUTPUT_playTone(tone, volume * 10, AUDIO_BOTHCHANNELS);
    }
}

unsigned int getHighScore() {
    char score[100];
    unsigned int score_int;
    SDCARD_readLine(store_filename, 100, score);
    score_int = strtoul(score, NULL, 10);
    return score_int;
}

void setHighScore(unsigned int new_high_score) {
    char* text;
    if (new_high_score > 10)
        new_high_score = 10;

    high_score = new_high_score;
    sprintf(text, "%u\r\n", new_high_score);
    SDCARD_writeToFile(store_filename, text);
}

// Resets all audio flags
void resetAudioFlags() {
    play_levelup_audio = true;
    play_gameover_audio = true;
    play_celebrate_audio = true;
}

// Sets the current state of game
// State can be one of:
// GAMEENGINE_MAINMENU, GAMEENGINE_PLAYING, GAMEENGINE_PAUSED
// GAMEENGINE_LEVELUP, GAMEENGINE_GAMEOVER, GAMEENGINE_WIN
void GameEngine_setState(unsigned int new_state) {
    // if state is unexpected value return to Main Menu
    if (new_state > GAMEENGINE_WIN)  // > 5
        new_state = GAMEENGINE_MAINMENU;

    // update game state
    state = new_state;
}

// Returns the current state of game
unsigned int GameEngine_getState() {
    return state;
}

// Sets the current level of game
void GameEngine_setLevel(unsigned int new_level) {
    unsigned int difficulty;

    // level cannot be higher than 9
    if (new_level > 9)
        new_level = 9;

    // update level
    level = new_level;

    // Generate a new question when the level is changed
    // This is a side effect but prevents constantly generating new questions
    // If on easy mode find difficulty from easy_question_levels else from hard_question_levels
    difficulty = game_mode == GAMEENGINE_EASY ? easy_question_levels[level] : hard_question_levels[level];

    // Generate new question with appropriate difficulty level
    current_question = QuestionGenerator_generateQuestion(difficulty);

    // Reset audio flags everytime the level is changed
    resetAudioFlags();
}

// Sets the current game mode of game
// Level can be one of: GAMEENGINE_EASY and GAMEENGINE_HARD
void GameEngine_setGameMode(unsigned int new_game_mode) {
    // Guard to check if new game mode is invalid
    if (new_game_mode > GAMEENGINE_HARD)
        new_game_mode = GAMEENGINE_EASY;

    // update difficulty
    game_mode = new_game_mode;

    if (game_mode == GAMEENGINE_EASY) {
        GameEngine_setTimeLimit(30.0);
    } else {
        GameEngine_setTimeLimit(15.0);
    }
}

// Sets the current score of game
void GameEngine_setScore(unsigned int new_score) {
    // new_score cannot be higher than 10
    if (new_score > 10)
        new_score = 10;

    // update the score
    score = new_score;
}

// Sets the time limit of each level of the game
void GameEngine_setTimeLimit(float new_time_limit) {
    // new_time_limit cannot be more than 30 seconds
    if (new_time_limit > 30.0)
        new_time_limit = 30.0;

    // update time limit
    time_limit = new_time_limit;
}

// Returns the time limit
float GameEngine_getTimeLimit() {
    return time_limit;
}

// Increases the volume by 1 unit up to 10
void GameEngine_increaseVolume() {
    if (!(volume > 10.0))
        volume += 1.0;
}

// Decreases the volume by 1 unit down to 0
void GameEngine_decreaseVolume() {
    if (!(volume < 0))
        volume -= 1.0;
}

// Set the current volume of the game
void GameEngine_setVolume(float new_volume) {
    // new_volume cannot be lower than 0
    if (new_volume < 0)
        new_volume = 0;

    // new_volume cannot be higher than 10
    if (new_volume > 10)
        new_volume = 10.0;

    // update volume
    volume = new_volume;
}

void GameEngine_setLevelUpLastUpdateTime(unsigned int time) {
    last_update_time[levelup] = time;
    last_update_time[levelup_text_display] = time;
    //    last_update_time[levelup_led_show] = Timer_getValueMS();
}

void GameEngine_setCelebrateLastUpdateTime(unsigned int time) {
    last_update_time[celebrate] = time;
    last_update_time[celebrate_led_show] = time;
    //    last_update_time[celebrate_text_display] = time;
}

void GameEngine_setGameOverLastUpdateTime(unsigned int time) {
    last_update_time[gameover] = time;
    last_update_time[gameover_text_display] = time;
    //    last_update_time[gameover_led_show] = time;
}

// Intialises the state variables of the game to default values
void GameEngine_initialise(char* storage_filename) {
    strcpy(store_filename, storage_filename);  // set the storage file name
    GameEngine_setLevel(0);                    // Start at level 0
    GameEngine_setGameMode(GAMEENGINE_EASY);   // Default game mode is Easy
    GameEngine_setScore(0);                    // Initial score is 0
    GameEngine_setState(GAMEENGINE_MAINMENU);  // Initial screen is Main Menu
    GameEngine_setVolume(5.0);                 // Default volume is 50%

    // check if storage file exists
    if (SDCARD_checkFileExists(store_filename)) {
        high_score = getHighScore();  // if exists get high score from SD Card
    } else {
        setHighScore(0);  // create file and add default highscore
    }
}

void GameEngine_reset() {
    // if wrong option is selected
    // update high score if current score is higher
    if (score > high_score)
        setHighScore(score);

    GameEngine_setScore(0);  // reset score to 0
    GameEngine_setLevel(0);  // reset level to 0
}

// Update game state based on selected MCQ option
// Called when player selects an MCQ options
// option can be one of: GAMEENGINE_OPTION1, GAMEENGINE_OPTION2,
// GAMEENGINE_OPTION3, GAMEENGINE_OPTION4
void GameEngine_enterOption(unsigned int option) {
    // Guard to check if game is in correct state i.e. PLAYING
    if (state != GAMEENGINE_PLAYING)
        return;

    // If option is same as answer i.e. correct answer is selected
    if (current_question.options[option] == current_question.answer) {
        GameEngine_setScore(score + 1);  // increment score by 1

        if (score >= 10) {
            GameEngine_reset();
            GameEngine_setCelebrateLastUpdateTime(Timer_getValueMS());
            GameEngine_setState(GAMEENGINE_WIN);  // display game over screen
        } else {
            GameEngine_setLevel(level + 1);  // increment level by 1
            GameEngine_setLevelUpLastUpdateTime(Timer_getValueMS());
            GameEngine_setState(GAMEENGINE_LEVELUP);  // display level up screen
        }

    } else {
        GameEngine_reset();
        GameEngine_setGameOverLastUpdateTime(Timer_getValueMS());
        GameEngine_setState(GAMEENGINE_GAMEOVER);  // display game over screen
    }
}

// Sets the color of the timer progress bar based on % of time left
void setTimerColor(float timer_value_percentage) {
    if (timer_value_percentage < 33) {
        // If 1/3 of time is left, set color to RED
        timer_color[0] = RED[0];
        timer_color[1] = RED[1];
        timer_color[2] = RED[2];
    } else if (timer_value_percentage < 66) {
        // If 2/3 of time is left, set color to RED
        timer_color[0] = ORANGE[0];
        timer_color[1] = ORANGE[1];
        timer_color[2] = ORANGE[2];
    } else {
        // else set color to GREEN
        timer_color[0] = GREEN[0];
        timer_color[1] = GREEN[1];
        timer_color[2] = GREEN[2];
    }
}

// Displays main menu screen
void GameEngine_displayMainMenu() {
    // draw main menu with game's current mode and volume level
    GraphicsEngine_drawMainMenu(game_mode, (unsigned int)volume, high_score);
    SevenSeg_displayWord("start");
    LED_write(0);
}

// Displays pause menu screen
void GameEngine_displayPauseMenu() {
    // draw pause menu with game's current volume level
    GraphicsEngine_drawPauseMenu((unsigned int)volume);
    LED_write(0);
}

// Displays question with four options and timer progress bar on the screen
unsigned int GameEngine_displayLevel(float time_remaining) {
    float timer_value_percentage;
    int option_id;
    // Set background to WHITE
    GraphicsEngine_setBackground(255, 255, 255);

    // Determine percentage of time_limit left and update timer color
    timer_value_percentage = ((time_remaining / time_limit) * 100);
    setTimerColor(timer_value_percentage);

    // draw timer progress bar with value and color
    GraphicsEngine_drawProgressBar(180, 40, 20, 240, timer_value_percentage, timer_color);
    // show time on servo
    Servo_setPositionInRange(0, 0, time_limit, time_remaining);

    // draw the current question on the screen
    GraphicsEngine_drawQuestion(current_question.text);

    // draw all 4 options on the screen
    for (option_id = 0; option_id < 4; option_id++) {
        char option_text[10];
        sprintf(option_text, "%u", current_question.options[option_id]);
        GraphicsEngine_drawOption(option_text, option_id);
    }

    // display current score on the seven segment displays
    SevenSeg_displayNumber(score);
    // display level on LEDs
    LED_setValueInRange(0, 10, level + 1);

    return GAMEENGINE_SUCCESS;
}

// If the answer is correct display Nice on the display
// Also run the leds from the left to the right
unsigned int GameEngine_levelUp() {
    unsigned int current_time;
    // Display "Level Up" on LCD
    GraphicsEngine_drawMessage("Level Up!", BLUE, WHITE, WHITE);
    // Display "correct ans" on seven segment
    displaySentence(level_up_text, levelup_text_display);
    // Show success led pattern
    //    GameEngine_levelUpLEDShow();

    // If audio hasn't been played, play it
    if (play_levelup_audio) {
        playSoundEffectFor(levelup_audio, C3, A3);
        // Once audio is played, unset flag so it does not play again
        // since it is a blocking operation
        play_levelup_audio = false;
    }

    // Get current timer value
    current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[levelup] - current_time) >= timer_update_period[levelup]) {
        GameEngine_setState(GAMEENGINE_PLAYING);

        // Update last update time value
        last_update_time[levelup] = current_time;
    }

    return GAMEENGINE_SUCCESS;
}

// This will cycle the LEDs through 2 leds to shift across the display
unsigned int GameEngine_levelUpLEDShow() {
    // Get current timer value
    unsigned int current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[levelup_led_show] - current_time) >= timer_update_period[levelup_led_show]) {
        // If reached end of pattern
        if (last_update_values[levelup_led_show] > 0x220) {
            last_update_values[levelup_led_show] = 0x011;  // reset led values
        }
        // Set leds to last display value
        LED_write(last_update_values[levelup_led_show]);
        // Update last display value to be inverse of current value << 1
        last_update_values[levelup_led_show] = last_update_values[levelup_led_show] << 1;
        // Update last update time value
        last_update_time[levelup_led_show] = current_time;
    }
    return GAMEENGINE_SUCCESS;
}

// If the answer is wrong display Failed and all
//  the leds will flash
unsigned int GameEngine_gameOver() {
    unsigned int current_time;
    // Display "Game Over" on LCD
    GraphicsEngine_drawMessage("Game Over!", RED, BLACK, BLACK);
    // Display "try again" on seven segment
    displaySentence(game_over_text, gameover_text_display);
    // Show failed led pattern
    //    GameEngine_gameOverLEDShow();

    // If audio hasn't been played, play it
    if (play_gameover_audio) {
        playSoundEffectFor(gameover_audio, G3, D3);
        // Once audio is played, unset flag so it does not play again
        // since it is a blocking operation
        play_gameover_audio = false;
    }

    // Get current timer value
    current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[gameover] - current_time) >= timer_update_period[gameover]) {
        GameEngine_reset();
        GameEngine_setState(GAMEENGINE_MAINMENU);

        // Update last update time value
        last_update_time[gameover] = current_time;
    }

    return GAMEENGINE_SUCCESS;
}

// This will display the leds for when the answer is wrong
unsigned int GameEngine_gameOverLEDShow() {
    // Get current timer value
    unsigned int current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[gameover_led_show] - current_time) >= timer_update_period[gameover_led_show]) {
        // Set leds to last display value
        LED_write(last_update_values[gameover_led_show]);
        // Update last display value to be inverse of current value
        last_update_values[gameover_led_show] = ~last_update_values[gameover_led_show];
        // Update last update time value
        last_update_time[gameover_led_show] = current_time;
    }
    return GAMEENGINE_SUCCESS;
}

unsigned int GameEngine_celebrate() {
    unsigned int current_time;
    // Display "Victory!" on LCD
    GraphicsEngine_drawMessage("Victory!", GREEN, BLACK, WHITE);
    // Display "victory" on seven segment
    displaySentence(celebrate_text, celebrate_text_display);
    // Show celebrate led pattern
    //    GameEngine_celebrateLEDShow();

    // If audio hasn't been played, play it
    if (play_celebrate_audio) {
        playSoundEffectFor(celebrate_audio, G3, E4);
        // Once audio is played, unset flag so it does not play again
        // since it is a blocking operation
        play_celebrate_audio = false;
    }

    // Get current timer value
    current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[celebrate] - current_time) >= timer_update_period[celebrate]) {
        GameEngine_reset();
        GameEngine_setState(GAMEENGINE_MAINMENU);

        // Update last update time value
        last_update_time[celebrate] = current_time;
    }

    return GAMEENGINE_SUCCESS;
}

// This will cycle the leds 3 leds used
unsigned int GameEngine_celebrateLEDShow() {
    // Get current timer value
    unsigned int current_time = Timer_getValueMS();
    // If enough time has passed
    if ((last_update_time[celebrate_led_show] - current_time) >= timer_update_period[celebrate_led_show]) {
        // If reached end of pattern
        if (last_update_values[celebrate_led_show] > 0x2A0) {
            last_update_values[celebrate_led_show] = 0x015;  // reset led values
        }
        // Set leds to last display value
        LED_write(last_update_values[celebrate_led_show]);
        // Update last display value to be inverse of current value << 1
        last_update_values[celebrate_led_show] = last_update_values[celebrate_led_show] << 1;

        // Update last update time value
        last_update_time[celebrate_led_show] = current_time;
    }
    return GAMEENGINE_SUCCESS;
}

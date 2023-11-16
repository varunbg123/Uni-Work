/**
 * Game Engine.h
 *
 * Implementation of Game engine functions in this file
 *
 * Varun Gonsalves, Emmanuel Leo, Kaif Kutchwala
 *
 * Date: 06/05/2023
 */

#include <stdbool.h>

#define GAMEENGINE_SUCCESS 1

// Define state variable values
#define GAMEENGINE_MAINMENU 0
#define GAMEENGINE_PLAYING 1
#define GAMEENGINE_PAUSED 2
#define GAMEENGINE_LEVELUP 3
#define GAMEENGINE_GAMEOVER 4
#define GAMEENGINE_WIN 5
#define GAMEENGINE_EASY 0
#define GAMEENGINE_HARD 1
#define GAMEENGINE_OPTION1 0
#define GAMEENGINE_OPTION2 1
#define GAMEENGINE_OPTION3 2
#define GAMEENGINE_OPTION4 3

/**
 * GameEngine_levelUp
 *
 * High Level that will display "correct ans" when question is
 * answered correctly.
 *
 */
unsigned int GameEngine_levelUp(void);

/**
 * GameEngine_gameOver
 *
 * High Level that will display "try again" when question is
 * answered incorrectly.
 *
 */
unsigned int GameEngine_gameOver(void);

/**
 * GameEngine_failLedShow
 *
 * High Level that will play an animation on the LEDs when
 * the player loses.
 *
 */
unsigned int GameEngine_gameOverLEDShow(void);

/**
 * GameEngine_successLedShow
 *
 * High Level that will play an animation on the LEDs when
 * the player progresses to the next level.
 *
 */
unsigned int GameEngine_levelUpLEDShow(void);

/**
 * GameEngine_celebrate
 *
 * High level that will display "victory" in a scrolling effect
 *
 */
unsigned int GameEngine_celebrate(void);

/**
 * GameEngine_celebrateLedShow
 *
 * High Level that will shift 3 leds when the game is complete
 *
 */
unsigned int GameEngine_celebrateLEDShow(void);

/**
 * GameEngine_setState
 *
 * Sets the current state of the game
 *
 * Inputs:
 *      new_state:  The state to set the game in, values can be one of:
 *                  GAMEENGINE_MAINMENU, GAMEENGINE_PLAYING, GAMEENGINE_PAUSED
 *                  GAMEENGINE_LEVELUP, GAMEENGINE_GAMEOVER, GAMEENGINE_WIN
 *
 */
void GameEngine_setState(unsigned int new_state);

/**
 * GameEngine_getState
 * Can be used to get the current state of the game.
 *
 * Output:
 *      Returns the current state of the game as an unsigned int.
 *
 */
unsigned int GameEngine_getState(void);

/**
 * GameEngine_setLevel
 *
 * Sets the current level of the game
 * Changing the level generates a new question at the
 * required difficulty as well.
 *
 * Inputs:
 *      new_level:  The level to set within range 0-9
 *
 */
void GameEngine_setLevel(unsigned int new_level);

/**
 * GameEngine_setGameMode
 *
 * Sets the current mode of the game i.e. easy or hard.
 *
 * Inputs:
 *      new_game_mode:  The mode to set, values can be one of:
 *                      GAMEENGINE_EASY or GAMEENGINE_HARD
 *
 */
void GameEngine_setGameMode(unsigned int new_game_mode);

/**
 * GameEngine_setScore
 *
 * Sets the current score of the game
 *
 * Inputs:
 *      new_score:  The score to set  within range 0-10
 *
 */
void GameEngine_setScore(unsigned int new_score);

/**
 * GameEngine_setTimeLimit
 *
 * Sets the time limit for each question of the game
 *
 * Inputs:
 *      new_time_limit:  The time limit to set  within range 0-30
 *
 */
void GameEngine_setTimeLimit(float new_time_limit);

/**
 * GameEngine_getTimeLimit
 *
 * Can be used to get the time limit for questions in the game.
 *
 * Output:
 *      Returns the current state of the game as a float.
 *
 */
float GameEngine_getTimeLimit(void);

/**
 * GameEngine_increaseVolume
 *
 * Increases the game volume level by 1 unit up to 10.
 *
 */
void GameEngine_increaseVolume(void);

/**
 * GameEngine_decreaseVolume
 *
 * Decreases the game volume level by 1 unit down to 0.
 *
 */
void GameEngine_decreaseVolume(void);

/**
 * GameEngine_setVolume
 *
 * Sets the volume level for the game within range 0-10
 *
 * Inputs:
 *      new_volume:  The volume level to set within range 0-10
 *
 */
void GameEngine_setVolume(float new_volume);

/**
 * GameEngine_initialise
 *
 * Intialises game's state to default values. This includes:
 * Setting the score to 0
 * Setting the level to 0
 * Setting the game mode to GAMEENGINE_EASY
 * Setting the state to GAMEENGINE_MAINMENU
 * Setting the volume to 5.0
 *
 * Input:
 *      storage_filname:    name of the file used for storage of highscore.
 *
 */
void GameEngine_initialise(char *storage_filename);

/**
 * GameEngine_enterOption
 *
 * Used to select an option for the current question.
 *
 * If the correct option is selected, the level is incremented,
 * score is incremented, and level up screen is displayed.
 *
 * If the wrong option is selected, score and level are reset,
 * high score is updated and game over screen is displayed.
 *
 * Inputs:
 *      option:  The option selected by the player, values can be:
 *               GAMEENGINE_OPTION1, GAMEENGINE_OPTION2,
 *               GAMEENGINE_OPTION3 or GAMEENGINE_OPTION4
 *
 */
void GameEngine_enterOption(unsigned int option);

/**
 * GameEngine_displayMainMenu
 *
 * Displays the main menu of the game.
 *
 */
void GameEngine_displayMainMenu(void);

/**
 * GameEngine_displayPauseMenu
 *
 * Displays the pause menu of the game.
 *
 */
void GameEngine_displayPauseMenu(void);

/**
 * GameEngine_displayLevel
 *
 * Displays the question, options and time remaining for
 * the current question.
 *
 *  * Inputs:
 *      time_remaining:  the amount of time remaining out of the time limit
 *
 */
unsigned int GameEngine_displayLevel(float time_remaining);

/**
 * GameEngine_reset
 *
 * Resets the game's state, level, and score.
 *
 */
void GameEngine_reset(void);

/**
 * GameEngine_setLevelUpLastUpdateTime
 *
 * Sets the last update time for the level up animations
 *
 *  * Inputs:
 *      time:  the time to set the last update time value to
 *
 */
void GameEngine_setLevelUpLastUpdateTime(unsigned int time);

/**
 * GameEngine_setCelebrateLastUpdateTime
 *
 * Sets the last update time for the celebrate animations
 *
 *  * Inputs:
 *      time:  the time to set the last update time value to
 *
 */
void GameEngine_setCelebrateLastUpdateTime(unsigned int time);

/**
 * GameEngine_setGameOverLastUpdateTime
 *
 * Sets the last update time for the game over animations
 *
 *  * Inputs:
 *      time:  the time to set the last update time value to
 *
 */
void GameEngine_setGameOverLastUpdateTime(unsigned int time);

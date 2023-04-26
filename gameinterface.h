//
// Created by Yannick Alfred Knoll on 26.02.22.
//
#include "gamestate.h"
#include "action.h"
#include "programstate.h"

/*
 * The functions defined in this file define an interface between
 * the runGameStep/gamestape provider and the graphics provider
 */

/*
 * Define how often the game state should be updated and
 * displayGameState(...) should be called (in ms)
 */
#define GAME_UPDATE_RATE_MS 100

/*
 * Start the game and enables state updates
 */
void startGame();

/*
 * Stop the game and disable state updates
 */
void stopGame();

/*
 * This is called when the game has finished
 */
void onGameOver(int score);

/*
 * Setup and initialize game instance
 */
void initGame();

/*
 * Exposed ONLY for testing purposes
 */
void runGameStep();

/*
 * Return the current gameState
 *
 * Exposed ONLY for testing purposes
 */
GameState _getGameState();

/*
 * Return the current command, if any
 *
 * Exposed ONLY for testing purposes
 */
GameCommand _getCommand();


/*
 * Return the current effects array
 *
 * Exposed ONLY for testing purposes
 */
GameEffect* _getEffects();


/*
 * Add a new side effect to the side effect queue
 *
 * Should be used ONLY internally
 */
void enqueueEffect(enum GameEffect effect);

/*
 * Should be called when a user has dispatched a command
 * to add the command to the commandQueue. It will be processed
 * with the next game loop cycle.
 *
 * command: the command to be added to the commandQueue
 */
void dispatchGameCommand(GameCommand command);

/*
 *  This function will be called for each new game state.
 *
 *  The implementation of this function should display the current gameState on the screen.
 *
 *  gameState: The current gameState
 */
void displayProgramState(ProgramPage page, MenuState menuState, GameState gameState, GameoverPageState gameoverPageState, HighscoreState highscoreState);

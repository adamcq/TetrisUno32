//
// Created by Yannick Alfred Knoll on 26.02.22.
//

#include "gamestate.h"
#include "gameinterface.h"
#include "core.h"
#include "utils.h"
#include "gamelogic.h"
#include "assets.h"
#include "env.h"
#include "io.h"
#include "gamevar.h"

void updateGame(GameCommand, GameEffect);

GameEffect popEffect();


/*
 * Default state each new game will have.
 */
const GameState INITIAL_GAME_STATE = {
        0,
        0,
        {0},
        {
                {
                        16,
                        0,
                        {
                                {0, 1, 0},
                                {1, 1, 1},
                                {0, 0, 0},
                        },
                        {}
                },
                {},
        }
};


GameCommand nextCommand = NONE;

/*
 * Should be called when a user has dispatched a command
 * to add the command to the commandQueue. It will be processed
 * with the next game loop cycle.
 *
 * command: the command to be added to the commandQueue
 */
void dispatchGameCommand(GameCommand command) {
    nextCommand = command;
}

#define EFFECT_QUEUE_LENGTH 5

/*
 * Keep track of the length of the effectQueue;
 */
int currentEffectQueueIndex = -1;

/*
 * Hold all effects that have been dispatched between game loop cycle t-1 and t.
 * Will be processed and reset once the next game loop cycle has been reached
 */
enum GameEffect effectQueue[EFFECT_QUEUE_LENGTH] = {IDLE, IDLE, IDLE, IDLE, IDLE};

void enqueueEffect(enum GameEffect effect) {
    if (currentEffectQueueIndex < EFFECT_QUEUE_LENGTH) effectQueue[++currentEffectQueueIndex] = effect;
}

/*
 * Pop and returns the latest effect, if available
 *
 * returns: Pointer to effect if available, or
 *          NULL pointer
 */
GameEffect popEffect() {
    if (currentEffectQueueIndex >= 0) {
        GameEffect t = effectQueue[currentEffectQueueIndex];
        effectQueue[currentEffectQueueIndex] = IDLE;
        currentEffectQueueIndex--;
        return t;
    } else return IDLE;
}

/*
 * Resets the commandQueue length counter.
 * Put placeholder command 'NONE' to all indexes
 */
void flushEffectQueue() {
    currentEffectQueueIndex = 0;
    int i;
    for (i = 0; i < EFFECT_QUEUE_LENGTH; i++) {
        effectQueue[i] = 0;
    }
}

/*
 * Hold the current game state
 */
GameState gameState;

GameState _getGameState() {
    return gameState;
}

GameCommand _getCommand() {
    return nextCommand;
}

GameEffect *_getEffects() {
    return effectQueue;
}

bool firstSpeedup = false, secondSpeedup = false;

/*
 * Initializes a new game. May also be used as reset
 */
void initGame() {
    firstSpeedup = false;
    secondSpeedup = false;
    gameState = INITIAL_GAME_STATE;
#if TEST_DATA
    gameState = TEST_GAME_STATE;
#endif
}

/*
 * Is continuously called when a game is active
 */
void runGameStep() {
    //Update game state with commands and effects
    updateGame(nextCommand, popEffect());

#if CHIPKIT_ENV
    //Speedup game
    if(!firstSpeedup && gameState.score >= SPEEDUP1_THRESHOLD) {
        setTimer(SPEEDUP1_FACTOR);
        firstSpeedup = true;
    }
    if(!secondSpeedup && gameState.score >= SPEEDUP2_THRESHOLD) {
        setTimer(SPEEDUP2_FACTOR);
        secondSpeedup = true;
    }
#endif

    //At the end, increment time;
    gameState.time += 1;
    //Dispatch display update
#if CHIPKIT_ENV
    dispatchDisplayUpdate();
#endif
}

void clearBlock(Block *block) {
    int i, j;
    for (i = 0; i < BLOCK_DIM; i++) {
        for (j = 0; j < BLOCK_DIM; j++) {
            (*block)[i][j] = 0;
        }
    }
}

Block *randomBlock() {
#if CHIPKIT_ENV
    return &BLOCKS[randint(6)];
#else
    return &BLOCKS[2];
#endif
}

GameCommand fromEffect(GameEffect effect) {
    switch (effect) {
        case LEFT_E:
            return LEFT;
        case RIGHT_E:
            return RIGHT;
        case DOWN_E:
            return DOWN;
        default:
            return NONE;
    }
}

/*
 * Is called every GAME_UPDATE_RATE_MS to update the gameState accordingly
 *
 * command: Array with commands to be processed in the current cycle
 * len: size of `commands` array
 */
void updateGame(GameCommand command, const GameEffect effect) {
    //Increase timer

    //Start initialize with new random next block
    if (isEmptyBlock(gameState.nextBlock))
        cpy2d(3, 3, &gameState.nextBlock, randomBlock());

    if (isEmptyBlock(gameState.playground.movingBlock.block))
        spawnBlock(&gameState.playground.movingBlock, &gameState.nextBlock);

    //Various collision checks
    if (gameState.playground.movingBlock.y) {
        if (isColliding(gameState.playground.movingBlock, gameState.playground.placedBlocks)
            || isHittingBottom(gameState.playground.movingBlock)) {
            placeBlock(gameState.playground.movingBlock, &gameState.playground.placedBlocks);
            if (removeFullLines(&gameState.playground.placedBlocks)) gameState.score += 5;
            else gameState.score += 1;
            if (isGameOver(gameState.playground.placedBlocks)) {
                onGameOver(gameState.score);
                return;
            }
            clearBlock(&gameState.playground.movingBlock.block);
        } else {
            fall(&gameState.playground.movingBlock);
        }
    } else fall(&gameState.playground.movingBlock);

    /*
     * Execute effect, if there is one
     * Effects are executed every cycle
     */
    if (effect != IDLE &&
        isValidCommand(fromEffect(effect), gameState.playground.movingBlock, gameState.playground.placedBlocks)) {
        executeEffect(effect, &gameState.playground.movingBlock);
    }

    /*
     * Execute command, if there has been dispatched one
     * Commands are only executed every fourth cycle.
     */
    if (command != NONE && gameState.time % 4 == 0) {
        if (isValidCommand(command, gameState.playground.movingBlock, gameState.playground.placedBlocks)) {
            executeCommand(command, &gameState.playground.movingBlock);
            if (command == ROTATE) {
                gameState.playground.movingBlock.boundary = computeBoundary(gameState.playground.movingBlock.block);
            }
        }
        //Clear command, since it has either been executed or should is invalid and should be dismissed
        nextCommand = NONE;
    }
}






//
// Created by Yannick Alfred Knoll on 02.03.22.
//

#include "stdbool.h"
#include "programstate.h"
#include "action.h"
#include "gameinterface.h"
#include "utils.h"
#include "env.h"
#include "io.h"

ProgramPage currentProgramPage = MENU;
MenuState currentMenuState = {MENU_ENTRY_START};
GameoverPageState currentGameoverPageState = {{"\0\0\0"}, 0, 0, {-1, -1, -1}};
HighscoreState currentHighscoreState = {
        {{"xxx", -1}, {"xxx", -1}, {"xxx", -1}}
};

bool executeMenuCommand(MenuCommand);

bool
executeGameOverCommand(GameoverPageCommand command, GameoverPageState *gameoverPageState, ProgramPage *programPage);

bool decodeAndRunCommand(ProgramCommand programCommand);

/*
 * Inform the display about a change of data and causes it to update
 */
void dispatchDisplayUpdate() {
#if CHIPKIT_ENV
    displayProgramState(currentProgramPage, currentMenuState, _getGameState(), currentGameoverPageState,
                        currentHighscoreState);
#endif
}

/*
 * Central entry point for any button inputs of the game
 */
void run(ProgramCommand programCommand) {
    //Fix wierd bug
    //if(currentProgramPage != GAME) setTimer(1);
    if (decodeAndRunCommand(programCommand)) {
        //If the command actually changed something on the display.
        dispatchDisplayUpdate();
    }
    //Since the game also runs when no command is executed, we manually run a game step
    if (currentProgramPage == GAME) {
        runGameStep();
    }
}

void stopGame() {
#if CHIPKIT_ENV
    setTimer(1);
#endif
}

void onGameOver(int score) {
    currentProgramPage = GAME_OVER;
    currentGameoverPageState.score = score;
    stopGame();
    dispatchDisplayUpdate();
}

bool decodeAndRunCommand(ProgramCommand programCommand) {
    //Decode command
    switch (currentProgramPage) {
        case MENU: {
            MenuCommand m;
            switch (programCommand) {
                case BTN2:
                    m = CURSOR_UP;
                    break;
                case BTN3:
                    m = CURSOR_DOWN;
                    break;
                case BTN4:
                    m = ENTER;
                    break;
                default:
                    return false;
            }
            return executeMenuCommand(m);
        }
        case GAME: {
            GameCommand m;
            switch (programCommand) {
                case BTN1:
                    m = ROTATE;
                    break;
                case BTN2:
                    m = RIGHT;
                    break;
                case BTN3:
                    m = LEFT;
                    break;
                case BTN4:
                    m = DOWN;
                    break;
                default:
                    return false;
            }
            dispatchGameCommand(m);
            //Since game is running asynchronous, it takes care of updating the screen itself
            return false;
        }
        case GAME_OVER: {
            GameoverPageCommand m;
            switch (programCommand) {
                case BTN1:
                    m = BACK;
                    break;
                case BTN2:
                    m = DEC;
                    break;
                case BTN3:
                    m = INC;
                    break;
                case BTN4:
                    m = ACCEPT;
                    break;
                default:
                    return false;
            }
            return executeGameOverCommand(m, &currentGameoverPageState, &currentProgramPage);
        }
        case HIGHSCORE:
            //Go back, no matter which button was pressed
            currentProgramPage = MENU;
            return true;
        default:
            return false;
    }
}

bool executeMenuCommand(MenuCommand command) {
    switch (command) {
        case CURSOR_DOWN:
            if (currentMenuState.menuEntry == MENU_ENTRY_START) {
                currentMenuState.menuEntry = MENU_ENTRY_SCORE;
                return true;
            }
            return false;
        case CURSOR_UP:
            if (currentMenuState.menuEntry == MENU_ENTRY_SCORE) {
                currentMenuState.menuEntry = MENU_ENTRY_START;
                return true;
            }
            return true;
        case ENTER:
            switch (currentMenuState.menuEntry) {
                case MENU_ENTRY_START:
#if SHOW_GAMEOVER
                    currentProgramPage = GAME_OVER;
#else
                    initGame();
                    currentProgramPage = GAME;
#endif
                    return true;
                case MENU_ENTRY_SCORE:
                    currentProgramPage = HIGHSCORE;
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }

}

#define ALPHABET_MAX_INDEX 25
char ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";


void updateEnteredChars(GameoverPageState *gameoverPageState) {
    int i;
    for (i = 0; i < HIGHSCORE_NAME_MAX_CHAR; i++) {
        int t = gameoverPageState->letterAlphabetIndexes[i];
        if (t != -1) gameoverPageState->enteredChars[i] = ALPHABET[t];
    }
}

void saveHighscore(const char name[], int score, HighscoreState *highscoreState) {
    int minScore = 9999, saveIndex = 0;
    int i;
    for (i = 0; i < HIGHSCORE_MAX_ENTRIES; i++) {
        int v = highscoreState->higscores[i].score;
        if (v < minScore) {
            minScore = v;
            saveIndex = i;
        }
    }

    if (score > minScore) {
        HighscoreEntry e = {{}, score};
        copyString(e.highscoreName, name);
        highscoreState->higscores[saveIndex] = e;
    }

    //TODO: check before to not only discard highscore
}

bool
executeGameOverCommand(GameoverPageCommand command, GameoverPageState *gameoverPageState, ProgramPage *programPage) {
    switch (command) {
        case INC: {
            int i = gameoverPageState->letterAlphabetIndexes[gameoverPageState->inputPointer];
            int r;
            if (i < 0 || i == ALPHABET_MAX_INDEX) {
                r = 0;
            } else {
                r = i + 1;
            }
            gameoverPageState->letterAlphabetIndexes[gameoverPageState->inputPointer] = r;
            updateEnteredChars(gameoverPageState);
            return true;
        }
        case DEC: {
            int i = gameoverPageState->letterAlphabetIndexes[gameoverPageState->inputPointer];
            int r;
            if (i < 0) {
                r = 0;
            } else if (i == 0) {
                r = ALPHABET_MAX_INDEX;
            } else {
                r = i - 1;
            }
            gameoverPageState->letterAlphabetIndexes[gameoverPageState->inputPointer] = r;
            updateEnteredChars(gameoverPageState);
            return true;
        }
        case BACK:
            if (gameoverPageState->inputPointer > 0)
                gameoverPageState->inputPointer--;
            else
                *programPage = MENU;
            return true;
        case ACCEPT:
            if (gameoverPageState->inputPointer == 2) {
                saveHighscore(gameoverPageState->enteredChars, gameoverPageState->score,
                              &currentHighscoreState);
                char empty[] = "\0\0\0";
                copyString(gameoverPageState->enteredChars, empty);
                gameoverPageState->inputPointer = 0;
                *programPage = MENU;
                return true;
            } else {
                gameoverPageState->inputPointer++;
                return true;
            }
        default:
            return false;
    }
}


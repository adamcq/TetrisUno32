//
// Created by Adam Borgula on 01.03.22.
//


#include <stdint.h>
#include "graphics.h"
#include "display.h"
#include "gameinterface.h"
#include "gamestate.h"
#include "programstate.h"
#include "core.h"
#include "utils.h"

void displayMenu(MenuState);
void displayHighscore(HighscoreState highscoreState);
void displayGameOver(GameoverPageState gameOverPageState);

/*
 * sets pixels for 4x4 tile with (x,y) being the top left corner in 32*128
 * (0,0) is top left corner of the display when having buttons in bottom right
 * tile is hollow
 */


void setTile(int x, int y) {
    int i,j;
    for (i = x; i < x + TILE_SIZE; i++) {
        if (i == x || i == x + TILE_SIZE - 1)
            for (j = y; j < y + TILE_SIZE; j++)
                display2d[i][DISPLAY_HEIGHT - 1 - j] = 1;
        else {
            display2d[i][DISPLAY_HEIGHT - y - 1] = 1;
            display2d[i][DISPLAY_HEIGHT - y - 4] = 1;
        }
    }
}

/*
 * sets pixels for 2x2 tile with (x,y) being the top left corner in 32*128
 * (0,0) is top left corner of the display when having buttons in bottom right
 */
void setNextTile(int x, int y) {
    int i,j;
    for (i = x; i < x + NEXT_TILE_SIZE; i++) {
        for (j = y; j < y + NEXT_TILE_SIZE; j++) {
            display2d[i][DISPLAY_HEIGHT - 1 - j] = 1;
        }
    }
}

/*
 * draws Block with (x,y) being the top left corner in 32 * 128
 * Block is drawn by calling setTile multiple times
 * (0,0) is top left corner of the display when having buttons in bottom right
 */
void drawBlock(int x, int y, Block data) {
    int i,j;
    for (i = 0; i < BLOCK_DIM; i++)
    {
        for (j = 0; j < BLOCK_DIM; j++)
        {
            if(data[i][j]) setTile((x+j*TILE_SIZE),(y+i*TILE_SIZE));
        }
    }
}

/*
 * draws nextBlock in upper right corner
 * Block is drawn by calling setNextTile multiple times
 */
void drawNextBlock(Block data) {
    int i,j;
    for (i = 0; i < BLOCK_DIM; i++)
    {
        for (j = 0; j < BLOCK_DIM; j++)
        {
            if(data[i][j]) setNextTile((DISPLAY_WIDTH - BLOCK_DIM * NEXT_TILE_SIZE + j * NEXT_TILE_SIZE),(i * NEXT_TILE_SIZE));
        }
    }
}
/*
 * accepts gameState as parameter and displays everything necessary
 */
void displayGameState(GameState gameState) {
    clearDisplay();

    // movingBlock
    drawBlock(gameState.playground.movingBlock.x, gameState.playground.movingBlock.y, gameState.playground.movingBlock.block);

    // placedBlock
    int i,j;
    for (i = 0; i < PLAYGROUND_HEIGHT; i++) {
        for (j = 0; j < PLAYGROUND_WIDTH; j++) {
            if (gameState.playground.placedBlocks[i][j] == 1) { // weird, now different flip
                setTile(j*TILE_SIZE,i*TILE_SIZE);
            }
        }
    }

    // score
    setPixelsForNumber(0,0,gameState.score);

    // nextBlock
    drawNextBlock(gameState.nextBlock);

    // apply changes to display
    display2DToDisplay();
    display_image(0,display);
}


void displayProgramState(ProgramPage page, MenuState menuState, GameState gameState, GameoverPageState gameOverPageState, HighscoreState highscoreState) {
    if (page == GAME) {
        displayGameState(gameState);
    }
    if (page == MENU) {
        displayMenu(menuState);
    }
    if (page == HIGHSCORE) {
        displayHighscore(highscoreState);
    }
    if (page == GAME_OVER) {
        displayGameOver(gameOverPageState);
    }
}

void printLine(int x, int y, int length) {
    int i;
    for (i = 0; i < length; i++) {
        display2d[x + i][toOldCoordinateSys(y)] = 1;
    }
}

/*
 * displays menu
 */
void displayMenu(MenuState menuState) {

    uint8_t lineSmooth = 1, linePlay = 5, lineScore = linePlay + 3;

    clearDisplay();

    printLine(0,lineSmooth * 8 - 2, 30);
    printLine(0,lineSmooth * 8 + 18, 30);

    displayVerticalString(0,0,lineSmooth,"smooth");
    displayVerticalString(0,0,lineSmooth + 1, "tetris");

    displayVerticalString(0,0,linePlay, " play");
    displayVerticalString(0,0,linePlay + 1, " game");

    displayVerticalString(0,0,lineScore, " show");
    displayVerticalString(3,0,lineScore + 1, "score");

    if (menuState.menuEntry == MENU_ENTRY_START) {
        pointToLine(linePlay, 17);
    }
    else if (menuState.menuEntry == MENU_ENTRY_SCORE) {
        pointToLine(lineScore, 17);
    }

    displayButton(13,2,ARROW_UP);
    displayButton(14,3,ARROW_DOWN);
    displayButton(15,4,TICK);

    display2DToDisplay();
    display_image(0,display);
}

void displayHighscore(HighscoreState highscoreState) {
    clearDisplay();
    int lineScores = 1;

    printLine(0,lineScores * 8 - 2, 30);
    printLine(0,lineScores * 8 + 10, 30);

    displayVerticalString(0,0,lineScores,"scores");

    int i;
    int lineFirstName = 8 * lineScores + 16;
    int lineFirstScore = lineFirstName + 8;
    for (i = 0; i < HIGHSCORE_MAX_ENTRIES; i++) {
        displayVerticalString(8, lineFirstName + i * 24,0, highscoreState.higscores[i].highscoreName);
        setPixelsForNumber(8,lineFirstScore + i * 24,highscoreState.higscores[i].score);
    }

    displayButton(15,0,HOME);

    display2DToDisplay();
    display_image(0,display);
}

void displayGameOver(GameoverPageState gameoverPageState) {
//    gameoverPageState.score INTEGER
//    gameoverPageState.enteredChars CHARACTER ARRAY
//    gameoverPageState.inputPointer UNSIGNED INT
//    gameoverPageState.letterAlphabetIndexes INT ARRAY
    clearDisplay();
    int lineGame = 1;
    int lineEnter = lineGame + 4;
    int lineInput = lineEnter + 3;
    int inputOffset = 8;

    printLine(0,lineGame * 8 - 2, 30);
    printLine(0,lineGame * 8 + 18, 30);


    displayVerticalString(0,0,lineGame," game");
    displayVerticalString(0,0,lineGame + 1," over");

    displayVerticalString(2,0,lineEnter,"enter");
    displayVerticalString(0,0,lineEnter + 1," name");

    displayVerticalString(inputOffset, 0, lineInput, gameoverPageState.enteredChars);

    printLine(inputOffset - 1 + gameoverPageState.inputPointer * 5, lineInput * 8 + 10, 6);

    displayButton(12,1,CROSS);
    displayButton(13,2,ARROW_DOWN);
    displayButton(14,3,ARROW_UP);
    displayButton(15,4,TICK);

    display2DToDisplay();
    display_image(0,display);
}

/*
 * inverts pixels in m * n block starting at (x,y) in 32 * 128
 */
void invertPixels(int x, int y, int width, int height) {
    int i, j;
    for (i = x; i < x + width; i++) {
        for (j = toOldCoordinateSys(y); j > toOldCoordinateSys(y + height); j--) {
            if (display2d[i][j] == 1)
                display2d[i][j] = 0;
            else display2d[i][j] = 1;
        }
    }
}
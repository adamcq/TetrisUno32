//
// Created by Adam Borgula on 01.03.22.
//


#include <stdint.h>


#define CHAR_WIDTH 4
#define CHAR_HEIGHT 7

#include "gamestate.h"

typedef enum ButtonFunctionality {
    ARROW_UP, ARROW_DOWN, TICK, CROSS, HOME
} ButtonFunctionality;

// displaying 3x3 Block
void setTile(int, int);
void drawTile(int, int);
void drawBlock(int, int, Block data);

// displaying 2x2 nextBlock
void setNextTile(int,int);
void drawNextBlock(Block data);

// displaying string
void displayLetter(int,int,int, int, char);
void displayVerticalString(int,int,int, char* letter);

// displaying chars
void displayButton(int,int,ButtonFunctionality);
void displayChar(int x, int y, int *arr, int m, int n);

// displaying numbers
void setPixelsForNumber(int,int,int);
void setPixelsForDigit(int,int,int,int);

// displaying menu
void printLine(int,int,int);
int toOldCoordinateSys(int);
void pointToLine(int, int);
void invertPixels(int,int,int,int);

// display highscore
//void displayHighscore(HighscoreState highscoreState);

// display gameover
//void displayGameOver(gameOverPageState);


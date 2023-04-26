//
// Created by Yannick Alfred Knoll on 01.03.22.
//

#include <stdint.h>
#include "gamestate.h"

#ifndef OTHER_TESTUTILS_H
#define OTHER_TESTUTILS_H

void printGameStruct(GameState);

void printMovingBlock(MovingBlock);

void printArr2d(int col, int row, uint8_t arr[col][row]);

void printBoundary(Boundary);

#endif //OTHER_TESTUTILS_H

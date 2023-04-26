//
// Created by Yannick Alfred Knoll on 28.02.22.
//
#include <stdbool.h>
#include "gamestate.h"
#include "action.h"

#ifndef PROJECT_GAMEMUTATE_H
#define PROJECT_GAMEMUTATE_H

Block* randomBlock();

void spawnBlock(MovingBlock* movingBlock, Block* nextBlock);

void rotateRight(Block tetrisBlock);

void fall(MovingBlock* movingBlock);

bool removeFullLines(PlacedBlocks* placedBlocks);

void placeBlock(MovingBlock movingBlock, PlacedBlocks* placedBlocks);

Boundary computeBoundary(Block);

bool isGameOver(PlacedBlocks placedBlocks);

bool isValidCommand(GameCommand, MovingBlock movingBlock, PlacedBlocks placedBlocks);

bool isColliding(MovingBlock movingBlock, PlacedBlocks placedBlocks);

bool isHittingBottom(MovingBlock movingBlock);

void executeCommand(GameCommand cmd, MovingBlock *movingBlock);

void executeEffect(GameEffect eff, MovingBlock *movingBlock);

#endif //PROJECT_GAMEMUTATE_H

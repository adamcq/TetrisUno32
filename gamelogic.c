//
// Created by Yannick Alfred Knoll on 28.02.22.
//

#include "gamelogic.h"
#include "gamestate.h"
#include "utils.h"
#include "action.h"
#include "gameinterface.h"


void fall(MovingBlock *movingBlock) {
    movingBlock->y++;
}

void rotateRight(Block tetrisBlock) {
    uint8_t i, j, temp;
    for (i = 0; i < BLOCK_DIM / 2; i++) { // from geeks4geeks
        for (j = i; j < BLOCK_DIM - i - 1; j++) {
            temp = (int) tetrisBlock[i][j];
            tetrisBlock[i][j] = tetrisBlock[BLOCK_DIM - 1 - j][i];
            tetrisBlock[BLOCK_DIM - 1 - j][i] = tetrisBlock[BLOCK_DIM - 1 - i][BLOCK_DIM - 1 - j];
            tetrisBlock[BLOCK_DIM - 1 - i][BLOCK_DIM - 1 - j] = tetrisBlock[j][BLOCK_DIM - 1 - i];
            tetrisBlock[j][BLOCK_DIM - 1 - i] = temp;
        }
    }
}

void spawnBlock(MovingBlock *movingBlock, Block *nextBlock) {
    movingBlock->x = 12;
    movingBlock->y = 0;
    movingBlock->boundary = computeBoundary(*nextBlock);
    cpy2d(3, 3, &movingBlock->block, nextBlock);
    cpy2d(3, 3, nextBlock, randomBlock());
}

/*
 * Copy current movingBlockA into placedBlocks (only where '1')
 * at the current (x,y) position
 */
void placeBlock(MovingBlock movingBlock, PlacedBlocks *placedBlocks) {
    //translate from 32*128 to 8*32
    int transX = movingBlock.x / TILE_SIZE;
    int transY = movingBlock.y / TILE_SIZE;
    int i, j;
    for (i = transY; i < transY + BLOCK_DIM; i++) {
        for (j = transX; j < transX + BLOCK_DIM; j++) {
            int t = movingBlock.block[i - transY][j - transX];
            if (t != 0) (*placedBlocks)[i][j] = 1;
        }
    }

}

Boundary computeBoundary(Block block) {
    int lowestY = 99, lowestX = 99, highestX = -1, highestY = -1;
    int y, x;
    for (y = 0; y < BLOCK_DIM; y++) {
        for (x = 0; x < BLOCK_DIM; x++) {
            if (block[y][x]) {
                if (y > highestY) highestY = y;
                if (y < lowestY) lowestY = y;
                if (x > highestX) highestX = x;
                if (x < lowestX) lowestX = x;
            }
        }
    }
    Point bottom = {highestX, highestY};
    Point top = {lowestX, lowestY};
    Boundary boundary = {top, bottom};
    return boundary;
}

bool isGameOver(PlacedBlocks placedBlocks) {
    int i;
    for (i = 0; i < PLAYGROUND_WIDTH; i++) {
        if (placedBlocks[0][i] || placedBlocks[1][i]) return true;
    }
    return false;
}

bool isTouchingAtOffset(int offX, int offY, MovingBlock movingBlock, PlacedBlocks placedBlocks) {
    int i, j;
    for (i = 0; i < BLOCK_DIM; i++) {
        for (j = 0; j < BLOCK_DIM; j++) {
            int t = movingBlock.block[i][j];
            if (t) {
                //translate from 32*128 to 8*32
                int transX = (movingBlock.x / TILE_SIZE) + j;
                int transY = (movingBlock.y / TILE_SIZE) + i;
                //check if a Block is placed beneath, left or right
                if (placedBlocks[transY + offY][transX + offX]) return true;
            }
        }
    }
    return false;
}


bool isValidCommand(GameCommand cmd, MovingBlock movingBlock, PlacedBlocks placedBlocks) {
    switch (cmd) {
        case LEFT:
            return (movingBlock.x / TILE_SIZE + movingBlock.boundary.top.x > 0) &&
                   !isTouchingAtOffset(-1, 0, movingBlock, placedBlocks);
        case RIGHT:
            return (movingBlock.x / TILE_SIZE + movingBlock.boundary.bottom.x + 1 < PLAYGROUND_WIDTH) &&
                   !isTouchingAtOffset(1, 0, movingBlock, placedBlocks);
        case ROTATE:
            /*
             * Rotate is prevented when the (logical) center of the block is
             * all the way at the left or right side
             *
             * Only allow rotation if block is not a 2x2 block
             */
            return !(movingBlock.x / TILE_SIZE + 1 <= 0 || movingBlock.x / TILE_SIZE + 1 > PLAYGROUND_WIDTH) &&
                   movingBlock.boundary.top.x - movingBlock.boundary.bottom.x !=
                   movingBlock.boundary.top.y - movingBlock.boundary.bottom.y
                   && !isTouchingAtOffset(1, 0, movingBlock, placedBlocks)
                   && !isTouchingAtOffset(-1, 0, movingBlock, placedBlocks);
        default :
            //All other commands should not be prevented atm
            return true;
    }
}

void moveLinesDown(int startY, PlacedBlocks *placedBlocks) {
    int x, y;
    for (y = startY - 1; y >= 0; y--) {
        bool emptyLine = true;
        for (x = 0; x < PLAYGROUND_WIDTH; x++) {
            if ((*placedBlocks)[y][x]) emptyLine = false;
            (*placedBlocks)[y + 1][x] = (*placedBlocks)[y][x];
        }
        if (emptyLine) break;
    }
}

bool removeFullLines(PlacedBlocks *placedBlocks) {
    int x, y;
    bool anyFull = false;
    for (y = 0; y < PLAYGROUND_HEIGHT; y++) {
        bool full = true;
        for (x = 0; x < PLAYGROUND_WIDTH; x++) {
            if (!(*placedBlocks)[y][x]) {
                full = false;
                break;
            }
        }
        if (full) {
            moveLinesDown(y, placedBlocks);
            anyFull = true;
        }
    }
    return anyFull;
}


bool isColliding(MovingBlock movingBlock, PlacedBlocks placedBlocks) {
    return isTouchingAtOffset(0, 1, movingBlock, placedBlocks);
}

bool isHittingBottom(MovingBlock movingBlock) {
    int y = movingBlock.y;
    if (y < DISPLAY_HEIGHT - BLOCK_DIM * TILE_SIZE) return false;
    int lowestYOffset = 0;
    int i, j;
    for (i = 0; i < BLOCK_DIM; i++) {
        for (j = 0; j < BLOCK_DIM; j++) {
            if (movingBlock.block[i][j]) lowestYOffset = i;
        }
    }
    return y + lowestYOffset * TILE_SIZE + TILE_SIZE >= DISPLAY_HEIGHT;
}

void executeEffect(GameEffect eff, MovingBlock *movingBlock) {
    switch (eff) {
        case LEFT_E:
            movingBlock->x--;
            break;
        case RIGHT_E:
            movingBlock->x++;
            break;
        case DOWN_E:
            movingBlock->y++;
            break;
        default:
            //Do nothing
            break;
    }
}

void executeCommand(GameCommand cmd, MovingBlock *movingBlock) {
    int i;
    switch (cmd) {
        case LEFT:
            movingBlock->x--;
            //This will affect three more cycles
            for (i = 0; i < 3; i++) enqueueEffect(LEFT_E);
            break;
        case RIGHT:
            movingBlock->x++;
            for (i = 0; i < 3; i++) enqueueEffect(RIGHT_E);
            break;
        case ROTATE:
            rotateRight(movingBlock->block);
            break;
        case DOWN:
            movingBlock->y++;
            for (i = 0; i < 3; i++) enqueueEffect(DOWN_E);
            break;
        default:
            //Do nothing since this should not happen
            break;
    }
}



//
// Created by Yannick Alfred Knoll on 26.02.22.
//


#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include <stdint.h>

#define PLAYGROUND_WIDTH 8
#define PLAYGROUND_HEIGHT 32
#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 128

#define BLOCK_DIM 3
#define TILE_SIZE 4
#define NEXT_TILE_SIZE 2

/*
 * Representation of a Block,
 */
typedef uint8_t Block[BLOCK_DIM][BLOCK_DIM];

/*
 * Internal representation of all blocks that
 * already have been placed on the 8x32 playground
 */

typedef uint8_t PlacedBlocks[PLAYGROUND_HEIGHT][PLAYGROUND_WIDTH];

/*
 * ==================================
 * GameState State
 * ==================================
 */

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Boundary {
    Point top;
    Point bottom;
} Boundary;

typedef struct MovingBlock {
    int x;
    int y;
    Block block;
    Boundary boundary;
} MovingBlock;

typedef struct Playground {
    MovingBlock movingBlock;
    PlacedBlocks placedBlocks;
} Playground;

typedef struct GameState {
    int time;
    int score;
    Block nextBlock;
    Playground playground;
} GameState;

/*
 * ==================================
 * GameState State end
 * ==================================
 */


#endif //PROJECT_GAME_H


#include <printf.h>
#include "gamestate.h"
#include "testutils.h"
#include "gamelogic.h"
#include "programstate.h"
#include "assets.h"

void runTest();

PlacedBlocks blocks = {0};
Block blockA = {
        {0, 0, 0},
        {0, 1, 1},
        {0, 1, 0}
};
MovingBlock movingBlockA = {
        1 * TILE_SIZE, 19 * TILE_SIZE, {
                {0, 1, 0},
                {1, 1, 0},
                {0, 0, 0}
        }
};
MovingBlock movingBlockB = {
        4 * TILE_SIZE, 0 * TILE_SIZE, {
                {1, 1, 0},
                {1, 1, 0},
                {0, 0, 0}
        }
};


void runTest() {
    HighscoreState s = { {{"xxx", -1},{"xxx", -1}, {"xxx", -1}}};
    currentHighscoreState = s;
    saveHighscore("LOL", 2, &s);
}


//
// Created by Yannick Alfred Knoll on 01.03.22.
//


#include <printf.h>
#include "testutils.h"
#include "utils.h"
#include "programstate.h"

void printArr2d(int col, int row, uint8_t arr[col][row]) {
    int i,j;
    for (i = 0; i < col; i++) {
        for (j = 0; j < row; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void printGameStruct(GameState game) {
    printf("Game {\n");
    printf(" time = %d\n", game.time);
    printf(" score = %d\n", game.score);
    printf(" movingBlockA = {\n");
    printf("   x = %d\n", game.playground.movingBlock.x);
    printf("   y = %d\n", game.playground.movingBlock.y);
    printMovingBlock(game.playground.movingBlock);
    printf(" }\n");
    printf("}\n");
}

void printMovingBlock(MovingBlock mv) {
    printf("   block = {\n");
    if (!isEmptyBlock(mv.block)) {
        for (int i = 0; i < 3; i++) {
            printf("        ");
            for (int j = 0; j < 3; j++) {
                printf("%d   ", mv.block[i][j]);
            }
            printf("\n");

        }
    } else {
        printf("      EMPTY\n");
    }
    printf("   }\n");
}

void printBoundary(Boundary boundary) {
    printf("Boundary {\n");
    printf("    top: (%d,%d)\n", boundary.top.x, boundary.top.y);
    printf("    bottom: (%d,%d)\n", boundary.bottom.x, boundary.bottom.y);
    printf("}\n");
}

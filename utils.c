//
// Created by Yannick Alfred Knoll on 27.02.22.
//

#include "utils.h"


void cpy2d(int cols, int rows, Block* dest, Block* src) {
    int i,j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            (*dest)[i][j] = (*src)[i][j];
        }
    }
}

void copyString(char dest[3], const char src[3]) {
    int i;
    for (i = 0; i < 3; i++) {
        dest[i] = src[i];
    }
}

/*
 * Checks if the Block is all zeros
 */
bool isEmptyBlock(Block block) {
    int i, j;
    for (i = 0; i < BLOCK_DIM; i++) {
        for (j = 0; j < BLOCK_DIM; j++) {
            if (block[i][j] == 1) return false;
        }
    }
    return true;
}
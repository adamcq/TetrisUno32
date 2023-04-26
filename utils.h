//
// Created by Yannick Alfred Knoll on 27.02.22.
//
#include "stdlib.h"
#include <stdbool.h>
#include "gamestate.h"

void cpy2d(int cols, int rows, Block* dest, Block* src);

void *memcpy(void *dest, const void *src, size_t n);

void copyString(char dest[3], const char src[3]);

bool isEmptyBlock(Block);





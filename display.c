//
// Created by Adam Borgula on 28.02.22.
//

#include <stdint.h>

uint8_t display2d[32][128];
uint8_t display[512];

void setDisplay2D(int x, int y, int width, int height) {
    int row, col;
    for (row = 0; row < 32; row++) {
        for (col = 0; col < 128; col++) {
            if (row >= y && row <= (y + height) && col >= x && col <= (x + width)) {
                display2d[row][col] = 1;
            }
        }
    }
}

void display2DToDisplay() {
    uint8_t box, seg;
    int page, col, row;

    for (page = 0; page < 4; page++) {
        for (col = 0; col < 128; col++) {
            box = 1; // 0000 0001
            seg = 0; // 0000 0000

            for (row = 0; row < 8; row++) {
                if (display2d[8 * page + row][col]) {
                    seg |= box; // activates box(bit) of the segment if desired
                }
                box *= 2; // move to next box e.g. 0000 0001 -> 0000 0010
            }
            display[page * 128 + col] = seg;
        }
    }
}


void clearDisplay() {
    int row, col, i;

    for (row = 0; row < 32; row++) {
        for (col = 0; col < 128; col++) {
            display2d[row][col] = 0;
        }
    }

    for (i = 0; i < 512; i++) {
        display[i] = 0;
    }
}

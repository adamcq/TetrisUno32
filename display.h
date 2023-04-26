//
// Created by Adam Borgula on 28.02.22.
//

#include <stdint.h>

extern uint8_t display2d[32][128];  // contains value of each of the 32(bit rows) * 128 (bit columns)
extern uint8_t display[512];        // contains value of each of the 4(byte page) * 128 (columns)

void display2DToDisplay(void);
void setDisplay2D(int, int, int, int);
void clearDisplay();
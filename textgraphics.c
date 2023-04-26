//
// Created by Adam Borgula on 02.02.22.
//


#include "graphics.h"
#include "display.h"
#include "core.h"
#include "programstate.h"

/*
* returns correct y-coordinate
*/
int toOldCoordinateSys(int y) {
    return DISPLAY_HEIGHT - y;
}

/*
 * displays noOfLines lines starting at the index line in negative (16 total lines)
 */
void pointToLine(int line, int noOfLines) {
    int i,j;

    // inverts bits
    for (i = toOldCoordinateSys(line * 8); i > toOldCoordinateSys(line * 8 + noOfLines); i--) {
        for (j = 0; j < DISPLAY_WIDTH - 1; j++) {
            if (display2d[j][i] == 1)
                display2d[j][i] = 0;
            else display2d[j][i] = 1;
        }
    }
}

/*
 * displaying explanation what each button stands for
 */



int tickChar[7][5] = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,0,0,1,0},
        {1,0,1,0,0},
        {0,1,0,0,0},
        {0,0,0,0,0}
};

int crossChar[7][5] = {
        {0,0,0,0,0},
        {1,0,0,0,1},
        {0,1,0,1,0},
        {0,0,1,0,0},
        {0,1,0,1,0},
        {1,0,0,0,1},
        {0,0,0,0,0}
};

int arrowUpChar[7][5] = {
        {0,0,1,0,0},
        {0,1,1,1,0},
        {1,0,1,0,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0}
};

int arrowDownChar[7][5] = {
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {1,0,1,0,1},
        {0,1,1,1,0},
        {0,0,1,0,0}
};

int homeChar[7][7] = {
        {0,0,0,1,0,0,0},
        {0,0,1,1,1,0,0},
        {0,1,1,1,1,1,0},
        {1,1,1,1,1,1,1},
        {0,1,1,1,1,1,0},
        {0,1,1,1,1,1,0},
        {0,1,1,0,1,1,0}
};


void displayButton(int line, int buttonNo, ButtonFunctionality buttonFunctionality) {
    int x = 25;
    int y = line * 8;

    if (buttonFunctionality != HOME) {
        displayVerticalString(0, 0, line, "b");
        setPixelsForNumber(CHAR_WIDTH + 1, line * 8, buttonNo);
    }

    switch (buttonFunctionality) {
        case ARROW_UP: // tick
            displayChar(x,y,*arrowUpChar, 7, 5);
            break;
        case ARROW_DOWN: // arrow up
            displayChar(x,y,*arrowDownChar, 7, 5);
            break;
        case TICK: // arrow down
            displayChar(x,y,*tickChar, 7, 5);
            break;
        case CROSS: // cross
            displayChar(x,y,*crossChar, 7, 5);
            break;
        case HOME:
            displayVerticalString(0,0,line,"btn");
            displayChar(x,y,*homeChar,7,7);
    }
}

/*
 * 2d array containing 26 letters of alphabet
 */
uint8_t alphabet[7][104] = {
        {0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,0,1,1,1,1},
        {1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1},
        {1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,0,0,0,1,0},
        {1,1,1,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,1,1,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
        {1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,1,0,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,0,0,0},
        {1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,1,1,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,0,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,0,0,1,1,1,1}
};

/*
 * Used from displayVerticalString function
 * Sets appropriate bits in display2d to a letter from alphabet
 * 3 parameters:
 * 1. lineY (y-coordinate in 32 * 128)
 * 2. offsetX (index in string, used to compute x-coordinate in 32 * 128)
 * 3. ascii lowercase letter or space
 * */
void displayLetter(int x, int y, int lineY, int offsetX, char c) {
    if (c == ' ')
        return;
    else {
        int i, j;
        for (i = 0; i < CHAR_WIDTH; i++) {
            for (j = toOldCoordinateSys(CHAR_HEIGHT + ((lineY) * (CHAR_HEIGHT + 1) + y)); j < toOldCoordinateSys((lineY) * (CHAR_HEIGHT + 1) + y); j++) {
                display2d[i + (CHAR_WIDTH + 1) * offsetX + x][j] = alphabet[toOldCoordinateSys(1 + ((lineY) * (CHAR_HEIGHT + 1) + y) + j)][CHAR_WIDTH * (c - 97) + i];
            }
        }
    }
}

/*
 * Displays a string on a specified line (0 to 15)
 */
void displayVerticalString(int x, int y, int line, char letter[]) {
    int letterIndex = 0;
    while (*letter != '\0') {
        displayLetter(x,y,line, letterIndex, *letter);
        letterIndex++;
        letter++;
    }
}

/*
 * 2d array containing all digits
 */
const uint8_t digits[CHAR_HEIGHT][CHAR_WIDTH * 10] = {
        {0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
        {1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1},
        {0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0}
};

/*
 * called by setPixelsForNumber
 * 1. digitOffset means which digit in a multidigit number it is (0 to 6)
 * 2. digit is self-explanatory
 * (x,y) in our 32 * 128 coordinate sys
 */
void setPixelsForDigit(int x, int y, int digitOffset, int digit) {

    int i, j;
    for (i = x; i < x + CHAR_WIDTH; i++) {
        for (j = toOldCoordinateSys(CHAR_HEIGHT + y); j < toOldCoordinateSys(y); j++) {
            display2d[i + (CHAR_WIDTH + 1) * digitOffset][j] = digits[toOldCoordinateSys(j + y + 1)][CHAR_WIDTH * digit + i - x];
        }
    }
}

/*
 * accepts digit as parameter and calls setPixelsForDigit digit by digit
 * (x,y) in our 32 * 128 coordinate sys
 */
void setPixelsForNumber(int x, int y, int n) {

    if (n == 0) {
        setPixelsForDigit(x,y,0,0);
    }

    // compute last digit index
    int i = 1;
    int powerOfTen = 0;
    while (i * 10 <= n) {
        powerOfTen++;
        i *= 10;
    }

    // print digit at specified index (powerOfTen)
    int mod;
    while(n > 0) //do till num greater than  0
    {
        mod = n % 10;  //split last digit from number
        setPixelsForDigit(x,y,powerOfTen--, mod);

        n = n / 10;    //divide num by 10. num /= 10 also a valid one
    }
}

/*
 * displays given m * n array on (x,y)
 */

void displayChar(int x, int y, int* arr, int m, int n) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            display2d[x + j][toOldCoordinateSys(y + i + 1)] =  *((arr+i*n) + j);
        }
    }
}
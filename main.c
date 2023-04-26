/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog
   2022-03-03 by Yannick A. Knoll
   2022-03-03 by Adam Borgula

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <stdbool.h>
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "core.h" /* Declatations for these labs */
#include "display.h"
#include "graphics.h"
#include "main.h"
#include "assets.h"
#include "io.h"
#include "gameinterface.h"
#include "env.h"

void *stdin, *stdout, *stderr;

MenuState testMenuState = {
        MENU_ENTRY_START
};

HighscoreState testHighscoreState = {
        {
                {"abc", 102},
                {"xyz", 992},
                {"ldf", 12},
        }
};

GameoverPageState testGameoverPageState = {
        {'a', 'b', '\0'},
        0
};

/*
 * This should ALWAYS be called when an interrupt is reached
 *
 * It is the central entry point for the whole program
 * and facilitates correct display and state updates
 */
int p1 = 0, p2 = 0, p3 = 0, p4 = 0;

void dispatchProgramCycle() {
    bool playingGame = currentProgramPage == GAME;
    bool anyButton = false;
    if (getbtn1() && p1 == 0 && !playingGame) {
        run(BTN1);
        p1 = 1;
        anyButton = true;
    } else p1 = 0;
    if(getbtn1() && playingGame) {
        anyButton = true;
        run(BTN1);
    }

    if (getbtn2() && p2 == 0 && !playingGame) {
        run(BTN2);
        p2 = 1;
        anyButton = true;
    } else p2 = 0;
    if(getbtn2() && playingGame) {
        anyButton = true;
       run(BTN2);
    }

    if (getbtn3() && p3 == 0 && !playingGame){
        run(BTN3);
        p3 = 1;
        anyButton = true;
    } else p3 = 0;
    if(getbtn3() && playingGame) {
        anyButton = true;
        run(BTN3);
    }

    if (getbtn4() && p4 == 0 && !playingGame) {
        run(BTN4);
        p4 = 1;
        anyButton = true;
    } else p4 = 0;
    if(getbtn4() && playingGame){
        anyButton = true;
        run(BTN4);
    }

    if(!anyButton && playingGame) run(VOID);

}

//For testing purposes only TODO: remove
void test() {
    displayProgramState(GAME_OVER, testMenuState, TEST_GAME_STATE, testGameoverPageState, testHighscoreState);
}

/* Interrupt Service Routine */
void user_isr(void) {
#if TEST_DISPLAY
    test();
#else
    dispatchProgramCycle();
#endif
    IFSCLR(0) = 0x0100; //ACKNOWLEDGE INTERRUPT
}


void initProject(void) {
    //We need to show the initial state on the display right when it is started
    dispatchDisplayUpdate();
}

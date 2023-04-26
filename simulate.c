//
// Created by Yannick Alfred Knoll on 27.02.22.
//

#include <printf.h>
#include "gamestate.h"
#include "gamelogic.h"
#include "gameinterface.h"
#include "testutils.h"
#include "test.c"
#include "utils.h"

#define SIMULATION_STEPS 4



void simulateGame(int, GameCommand[SIMULATION_STEPS]);

void runSim();

/*
 * Run with:
 * gcc test.c gameloop.c gamemutate.c utils.c -v && ./a.out
 */


int main() {
    runTest();
    return 0;
}


void runSim() {
    GameCommand commands[SIMULATION_STEPS] = {LEFT, ROTATE, RIGHT, DOWN};
    initGame();
    simulateGame(SIMULATION_STEPS, commands);
}


void simulateGame(int steps, GameCommand command[SIMULATION_STEPS]) {
    for (int i = 0; i < steps * 4; i++) {
        printf("=====%d/%d=====\n", i, steps*4-1);
        //if(i % 4 == 0) dispatchGameCommand(command[i/4]);
        printf("GameCommand: %d\n", _getCommand());
        printf("Effects: ");
        for (int j = 0; j < 5; ++j) {
            printf("%d ", _getEffects()[j]);
        }
        printf("\n");
        runGameStep();
        printGameStruct(_getGameState());
    }
}


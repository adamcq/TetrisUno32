//
// Created by Yannick Alfred Knoll on 28.02.22.
//
#ifndef OTHER_ACTION_H
#define OTHER_ACTION_H

#include <stdint.h>


/*
 * Describes all possible commands that a player can dispatch
 * to change the game state
 */
typedef enum GameCommand {
    /*
     * Special command type.
     * Used as palceholder
     */
    NONE,

    /*
     * Ordinary commands listed below
     */
    DOWN,
    ROTATE,
    LEFT,
    RIGHT,


} GameCommand;


/*
 * Describes an internal effect enables to affect commands in one cycle
 * to affect multiple cycles in the future
 *
 * Example: Used for LEFT, RIGHT or DOWN movement, since a LEFT, RIGHT movement is animated
 * across multiple cycles.
 *
 */
typedef enum GameEffect {
    IDLE,
    LEFT_E,
    RIGHT_E,
    DOWN_E
} GameEffect;

#endif //OTHER_ACTION_H
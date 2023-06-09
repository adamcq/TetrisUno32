//
// Created by Yannick Alfred Knoll on 28.02.22.
//

#include "assets.h"

Block BLOCKS[6] = {{{0, 1, 0},
                           {0, 1, 0},
                           {1, 1, 0}},

                   {{0, 1, 0},
                           {0, 1, 0},
                           {0, 1, 1}},

                   {{0, 1, 0},
                           {1, 1, 1},
                           {0, 0, 0}},

                   {{1, 1, 0},
                           {1, 1, 0},
                           {0, 0, 0}},

                   {{1, 1, 0},
                           {0, 1, 1},
                           {0, 0, 0}},

                   {{0, 1, 1},
                           {1, 1, 0},
                           {0, 0, 0}}};

GameState TEST_GAME_STATE = {
        0,
        0,
        {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0},
        },
        {
                {
                        12,
                        0,
                        .block =  {
                                {1, 0, 0},
                                {1, 1, 0},
                                {0, 1, 0},
                        },
                        {}
                },
                {
                        {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {0, 0, 0, 1, 0, 0, 0, 0},
                         {1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 0, 0, 0},
                         {1, 1, 1, 1, 0, 0, 1, 0},
                         {1, 1, 1, 1, 0, 1, 1, 1},
                         {1, 1, 0, 1, 1, 1, 1, 1}
                        }
                }
        };
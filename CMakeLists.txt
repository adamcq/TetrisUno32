cmake_minimum_required(VERSION 3.21)
project(test C)
project(other C)

set(CMAKE_C_STANDARD 99)

add_executable(sim simulate.c assets.c assets.c gameloop.c gamelogic.c utils.c assets.c assets.h testutils.c testutils.h programstate.h programstate.c env.h gamevar.h)
add_executable(test test.c gameloop.c gamelogic.c utils.c testutils.c testutils.h)
add_executable(other core.c textgraphics.c display.c graphics.c io.c main.c init.c env.h gamevar.h)

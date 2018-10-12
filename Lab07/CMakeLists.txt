cmake_minimum_required(VERSION 3.8)

project(deadlockGraph)

set(CMAKE_C_STANDARD 11)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -g -O0 -D_DEBUG -Wall")

add_executable(
    deadlock
        main.c
        graph.c
)
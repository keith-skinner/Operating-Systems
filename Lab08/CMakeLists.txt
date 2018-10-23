#/**
#* Name: Keith Skinner
#* Lab/task: Lab 8
#* Date: 10/18/2018
#**/

project(inverted)

set(CMAKE_C_STANDARD 11)

SET(CMAKE_C_FLAGS "-m64 -g -O0 -D_DEBUG -Wall")

add_executable(inverted inverted.c testInverted.c)




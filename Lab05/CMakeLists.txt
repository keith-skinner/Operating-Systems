cmake_minimum_required(VERSION 3.8)

project(cpuScheduler)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-m64 -g -O0 -D_DEBUG -Wall")

add_executable(cpuScheduler cpuScheduler.c processQueue.c)

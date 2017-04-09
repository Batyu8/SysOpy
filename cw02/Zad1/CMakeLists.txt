cmake_minimum_required(VERSION 3.0)
project(cw02)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g -Wall -std=c99")

set(SOURCE_FILES src/main.c)
add_executable(Zad1 ${SOURCE_FILES})
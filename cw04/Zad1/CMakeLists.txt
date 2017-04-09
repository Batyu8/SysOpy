cmake_minimum_required(VERSION 3.6)
project(Zad1)

set(CMAKE_C_FLAGS "-Wall -g -std=gnu99")

set(SOURCE_FILES src/main.c)
add_executable(Zad1 ${SOURCE_FILES})
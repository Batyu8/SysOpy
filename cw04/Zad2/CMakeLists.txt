cmake_minimum_required(VERSION 3.0)
project(Zad2)

set(CMAKE_C_FLAGS "-Wall -g -std=gnu99")

set(SOURCE_FILES src/main.c)
add_executable(Zad2 ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.0)
project(Zad1)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g -Wall -std=c99")

set(SOURCES src/cbook.c)
add_library(contactbook_st STATIC ${SOURCES})
add_library(contactbook_sh SHARED ${SOURCES})

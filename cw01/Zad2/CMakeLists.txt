cmake_minimum_required(VERSION 3.0)
project(Zad2)

include_directories(include)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g -Wall -std=c99")

set(MAIN_SOURCES src/main.c)
set(LIB_SOURCES lib/cbook.c)

add_library(contactbook_sh SHARED ${LIB_SOURCES})
add_executable(Zad2sh ${MAIN_SOURCES})
target_link_libraries(Zad2sh contactbook_sh)




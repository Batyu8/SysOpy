cmake_minimum_required(VERSION 3.0)
project(Zad2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g -Wall -std=gnu99")
set(MAIN_SOURCE src/main.c)
set(UTIL_SOURCE src/print-env.c)
set(INF_SOURCE src/inf.c)

add_executable(Zad2 ${MAIN_SOURCE})
add_executable(print_env ${UTIL_SOURCE})
add_executable(inf ${INF_SOURCE})
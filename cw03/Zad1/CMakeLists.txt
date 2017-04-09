cmake_minimum_required(VERSION 3.6)
project(Zad1)

set(CMAKE_C_STANDARD 99)

set(MAIN_SOURCE src/main.c)
set(UTIL_SOURCE src/print_env.c)
add_executable(Zad1 ${MAIN_SOURCE})
add_executable(print_env  ${UTIL_SOURCE})
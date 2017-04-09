cmake_minimum_required(VERSION 3.0)
project(Zad2)

set(CMAKE_C_FLAGS "-Wall -g -std=gnu99")

set(SOURCE src/main.c)
set(SOURCE-NFTW src/main-nftw.c)
add_executable(Zad2 ${SOURCE})
add_executable(Zad2-nftw ${SOURCE-NFTW})
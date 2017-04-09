cmake_minimum_required(VERSION 3.0)
project(Zad3)

include_directories(include)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g -Wall -std=c99")

set(MAIN_SOURCES src/main.c)
set(MAIN_DLL_SOURCES src/maindll.c)
set(LIB_SOURCES lib/cbook.c)

IF(OPT STREQUAL "O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
ENDIF(OPT STREQUAL "O0")
IF(OPT STREQUAL "O1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
ENDIF(OPT STREQUAL "O1")
IF(OPT STREQUAL "O2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
ENDIF(OPT STREQUAL "O2")
IF(OPT STREQUAL "O3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
ENDIF(OPT STREQUAL "O3")
IF(OPT STREQUAL "Os")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
ENDIF(OPT STREQUAL "Os")


add_library(contactbook_st STATIC ${LIB_SOURCES})
add_executable(Zad3st ${MAIN_SOURCES})
target_link_libraries(Zad3st contactbook_st)

add_library(contactbook_sh SHARED ${LIB_SOURCES})
add_executable(Zad3sh ${MAIN_SOURCES})
target_link_libraries(Zad3sh contactbook_sh)

add_executable(Zad3dll ${MAIN_DLL_SOURCES})
target_link_libraries(Zad3dll dl)






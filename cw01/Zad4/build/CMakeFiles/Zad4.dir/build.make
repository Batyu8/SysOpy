# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build

# Include any dependencies generated for this target.
include CMakeFiles/Zad4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Zad4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Zad4.dir/flags.make

CMakeFiles/Zad4.dir/src/main.c.o: CMakeFiles/Zad4.dir/flags.make
CMakeFiles/Zad4.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Zad4.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Zad4.dir/src/main.c.o   -c /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/src/main.c

CMakeFiles/Zad4.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Zad4.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/src/main.c > CMakeFiles/Zad4.dir/src/main.c.i

CMakeFiles/Zad4.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Zad4.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/src/main.c -o CMakeFiles/Zad4.dir/src/main.c.s

CMakeFiles/Zad4.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/Zad4.dir/src/main.c.o.requires

CMakeFiles/Zad4.dir/src/main.c.o.provides: CMakeFiles/Zad4.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/Zad4.dir/build.make CMakeFiles/Zad4.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/Zad4.dir/src/main.c.o.provides

CMakeFiles/Zad4.dir/src/main.c.o.provides.build: CMakeFiles/Zad4.dir/src/main.c.o


# Object files for target Zad4
Zad4_OBJECTS = \
"CMakeFiles/Zad4.dir/src/main.c.o"

# External object files for target Zad4
Zad4_EXTERNAL_OBJECTS =

Zad4: CMakeFiles/Zad4.dir/src/main.c.o
Zad4: CMakeFiles/Zad4.dir/build.make
Zad4: libcontactbook_sh.so
Zad4: CMakeFiles/Zad4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Zad4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Zad4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Zad4.dir/build: Zad4

.PHONY : CMakeFiles/Zad4.dir/build

CMakeFiles/Zad4.dir/requires: CMakeFiles/Zad4.dir/src/main.c.o.requires

.PHONY : CMakeFiles/Zad4.dir/requires

CMakeFiles/Zad4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Zad4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Zad4.dir/clean

CMakeFiles/Zad4.dir/depend:
	cd /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4 /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4 /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build /home/bkoniecz/CLionProjects/KoniecznyBartlomiej/cw01/Zad4/build/CMakeFiles/Zad4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Zad4.dir/depend


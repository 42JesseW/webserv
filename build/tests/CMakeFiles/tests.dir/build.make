# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/danali/Documents/Codam/webserv_dev

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/danali/Documents/Codam/webserv_dev/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/tests.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/tests.dir/flags.make

tests/CMakeFiles/tests.dir/unittests/test_config.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/unittests/test_config.cpp.o: ../tests/unittests/test_config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/tests.dir/unittests/test_config.cpp.o"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/unittests/test_config.cpp.o -c /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_config.cpp

tests/CMakeFiles/tests.dir/unittests/test_config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/unittests/test_config.cpp.i"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_config.cpp > CMakeFiles/tests.dir/unittests/test_config.cpp.i

tests/CMakeFiles/tests.dir/unittests/test_config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/unittests/test_config.cpp.s"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_config.cpp -o CMakeFiles/tests.dir/unittests/test_config.cpp.s

tests/CMakeFiles/tests.dir/unittests/test_options.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/unittests/test_options.cpp.o: ../tests/unittests/test_options.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/tests.dir/unittests/test_options.cpp.o"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/unittests/test_options.cpp.o -c /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_options.cpp

tests/CMakeFiles/tests.dir/unittests/test_options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/unittests/test_options.cpp.i"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_options.cpp > CMakeFiles/tests.dir/unittests/test_options.cpp.i

tests/CMakeFiles/tests.dir/unittests/test_options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/unittests/test_options.cpp.s"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_options.cpp -o CMakeFiles/tests.dir/unittests/test_options.cpp.s

tests/CMakeFiles/tests.dir/unittests/test_request.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/unittests/test_request.cpp.o: ../tests/unittests/test_request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/tests.dir/unittests/test_request.cpp.o"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/unittests/test_request.cpp.o -c /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_request.cpp

tests/CMakeFiles/tests.dir/unittests/test_request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/unittests/test_request.cpp.i"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_request.cpp > CMakeFiles/tests.dir/unittests/test_request.cpp.i

tests/CMakeFiles/tests.dir/unittests/test_request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/unittests/test_request.cpp.s"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv_dev/tests/unittests/test_request.cpp -o CMakeFiles/tests.dir/unittests/test_request.cpp.s

# Object files for target tests
tests_OBJECTS = \
"CMakeFiles/tests.dir/unittests/test_config.cpp.o" \
"CMakeFiles/tests.dir/unittests/test_options.cpp.o" \
"CMakeFiles/tests.dir/unittests/test_request.cpp.o"

# External object files for target tests
tests_EXTERNAL_OBJECTS =

tests/tests: tests/CMakeFiles/tests.dir/unittests/test_config.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/unittests/test_options.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/unittests/test_request.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/build.make
tests/tests: tests/libtester.a
tests/tests: tests/CMakeFiles/tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/danali/Documents/Codam/webserv_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable tests"
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/tests.dir/build: tests/tests

.PHONY : tests/CMakeFiles/tests.dir/build

tests/CMakeFiles/tests.dir/clean:
	cd /home/danali/Documents/Codam/webserv_dev/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/tests.dir/clean

tests/CMakeFiles/tests.dir/depend:
	cd /home/danali/Documents/Codam/webserv_dev/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/danali/Documents/Codam/webserv_dev /home/danali/Documents/Codam/webserv_dev/tests /home/danali/Documents/Codam/webserv_dev/build /home/danali/Documents/Codam/webserv_dev/build/tests /home/danali/Documents/Codam/webserv_dev/build/tests/CMakeFiles/tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/tests.dir/depend

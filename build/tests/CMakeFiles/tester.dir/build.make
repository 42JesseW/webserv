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
CMAKE_SOURCE_DIR = /home/danali/Documents/Codam/webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/danali/Documents/Codam/webserv/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/tester.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/tester.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/tester.dir/flags.make

tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.o: ../srcs/CGI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/CGI.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/CGI.cpp

tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/CGI.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/CGI.cpp > CMakeFiles/tester.dir/__/srcs/CGI.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/CGI.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/CGI.cpp -o CMakeFiles/tester.dir/__/srcs/CGI.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.o: ../srcs/Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Client.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Client.cpp

tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Client.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Client.cpp > CMakeFiles/tester.dir/__/srcs/Client.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Client.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Client.cpp -o CMakeFiles/tester.dir/__/srcs/Client.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.o: ../srcs/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Config.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Config.cpp

tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Config.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Config.cpp > CMakeFiles/tester.dir/__/srcs/Config.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Config.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Config.cpp -o CMakeFiles/tester.dir/__/srcs/Config.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o: ../srcs/ConfigUtil.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/ConfigUtil.cpp

tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/ConfigUtil.cpp > CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/ConfigUtil.cpp -o CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.o: ../srcs/Option.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Option.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Option.cpp

tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Option.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Option.cpp > CMakeFiles/tester.dir/__/srcs/Option.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Option.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Option.cpp -o CMakeFiles/tester.dir/__/srcs/Option.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.o: ../srcs/Route.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Route.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Route.cpp

tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Route.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Route.cpp > CMakeFiles/tester.dir/__/srcs/Route.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Route.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Route.cpp -o CMakeFiles/tester.dir/__/srcs/Route.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.o: ../srcs/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Server.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Server.cpp

tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Server.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Server.cpp > CMakeFiles/tester.dir/__/srcs/Server.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Server.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Server.cpp -o CMakeFiles/tester.dir/__/srcs/Server.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.o: ../srcs/Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Socket.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Socket.cpp

tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Socket.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Socket.cpp > CMakeFiles/tester.dir/__/srcs/Socket.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Socket.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Socket.cpp -o CMakeFiles/tester.dir/__/srcs/Socket.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.o: ../srcs/Utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/Utils.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/Utils.cpp

tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/Utils.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/Utils.cpp > CMakeFiles/tester.dir/__/srcs/Utils.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/Utils.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/Utils.cpp -o CMakeFiles/tester.dir/__/srcs/Utils.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/main.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/main.cpp.o: ../srcs/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/main.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/main.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/main.cpp

tests/CMakeFiles/tester.dir/__/srcs/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/main.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/main.cpp > CMakeFiles/tester.dir/__/srcs/main.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/main.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/main.cpp -o CMakeFiles/tester.dir/__/srcs/main.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o: ../srcs/request/ErrorHandling.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/request/ErrorHandling.cpp

tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/request/ErrorHandling.cpp > CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/request/ErrorHandling.cpp -o CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o: ../srcs/request/HandleRequest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/request/HandleRequest.cpp

tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/request/HandleRequest.cpp > CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/request/HandleRequest.cpp -o CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o: ../srcs/request/Request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/request/Request.cpp

tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/request/Request.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/request/Request.cpp > CMakeFiles/tester.dir/__/srcs/request/Request.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/request/Request.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/request/Request.cpp -o CMakeFiles/tester.dir/__/srcs/request/Request.cpp.s

tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o: tests/CMakeFiles/tester.dir/flags.make
tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o: ../srcs/response/Response.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o -c /home/danali/Documents/Codam/webserv/srcs/response/Response.cpp

tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/__/srcs/response/Response.cpp.i"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danali/Documents/Codam/webserv/srcs/response/Response.cpp > CMakeFiles/tester.dir/__/srcs/response/Response.cpp.i

tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/__/srcs/response/Response.cpp.s"
	cd /home/danali/Documents/Codam/webserv/build/tests && /bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danali/Documents/Codam/webserv/srcs/response/Response.cpp -o CMakeFiles/tester.dir/__/srcs/response/Response.cpp.s

# Object files for target tester
tester_OBJECTS = \
"CMakeFiles/tester.dir/__/srcs/CGI.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Client.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Config.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Option.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Route.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Server.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Socket.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/Utils.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/main.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o" \
"CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o"

# External object files for target tester
tester_EXTERNAL_OBJECTS =

tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/CGI.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Client.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Config.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/ConfigUtil.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Option.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Route.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Server.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Socket.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/Utils.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/main.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/request/ErrorHandling.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/request/HandleRequest.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/request/Request.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/__/srcs/response/Response.cpp.o
tests/libtester.a: tests/CMakeFiles/tester.dir/build.make
tests/libtester.a: tests/CMakeFiles/tester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/danali/Documents/Codam/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX static library libtester.a"
	cd /home/danali/Documents/Codam/webserv/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/tester.dir/cmake_clean_target.cmake
	cd /home/danali/Documents/Codam/webserv/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/tester.dir/build: tests/libtester.a

.PHONY : tests/CMakeFiles/tester.dir/build

tests/CMakeFiles/tester.dir/clean:
	cd /home/danali/Documents/Codam/webserv/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/tester.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/tester.dir/clean

tests/CMakeFiles/tester.dir/depend:
	cd /home/danali/Documents/Codam/webserv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/danali/Documents/Codam/webserv /home/danali/Documents/Codam/webserv/tests /home/danali/Documents/Codam/webserv/build /home/danali/Documents/Codam/webserv/build/tests /home/danali/Documents/Codam/webserv/build/tests/CMakeFiles/tester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/tester.dir/depend


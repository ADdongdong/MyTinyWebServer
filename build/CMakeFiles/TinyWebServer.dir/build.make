# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build

# Include any dependencies generated for this target.
include CMakeFiles/TinyWebServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TinyWebServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TinyWebServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TinyWebServer.dir/flags.make

CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o: /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/tinyWebserver/log/log.cpp
CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o -MF CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o.d -o CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o -c /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/tinyWebserver/log/log.cpp

CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/tinyWebserver/log/log.cpp > CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.i

CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/tinyWebserver/log/log.cpp -o CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.s

# Object files for target TinyWebServer
TinyWebServer_OBJECTS = \
"CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o"

# External object files for target TinyWebServer
TinyWebServer_EXTERNAL_OBJECTS =

/home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/lib/libTinyWebServer.so: CMakeFiles/TinyWebServer.dir/tinyWebserver/log/log.cpp.o
/home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/lib/libTinyWebServer.so: CMakeFiles/TinyWebServer.dir/build.make
/home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/lib/libTinyWebServer.so: CMakeFiles/TinyWebServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/lib/libTinyWebServer.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TinyWebServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TinyWebServer.dir/build: /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/lib/libTinyWebServer.so
.PHONY : CMakeFiles/TinyWebServer.dir/build

CMakeFiles/TinyWebServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TinyWebServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TinyWebServer.dir/clean

CMakeFiles/TinyWebServer.dir/depend:
	cd /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build /home/hahadong/08_CPP_and_C_project/02_webserver/03_myTinyWebServer/build/CMakeFiles/TinyWebServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TinyWebServer.dir/depend


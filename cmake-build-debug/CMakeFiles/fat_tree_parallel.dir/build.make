# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /snap/clion/107/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/107/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/fat_tree_parallel.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fat_tree_parallel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fat_tree_parallel.dir/flags.make

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o: ../experiment/fat_tree/fat_tree_parallel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_parallel.cpp

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_parallel.cpp > CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.i

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_parallel.cpp -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.s

CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o: ../graph/Graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/Graph.cpp

CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/Graph.cpp > CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.i

CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/Graph.cpp -o CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.s

CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o: ../graph/fattree/FatTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/fattree/FatTree.cpp

CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/fattree/FatTree.cpp > CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.i

CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/graph/fattree/FatTree.cpp -o CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.s

CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o: ../node/FatTreeNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/node/FatTreeNode.cpp

CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/node/FatTreeNode.cpp > CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.i

CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/node/FatTreeNode.cpp -o CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.s

CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o: ../utils/FatTreeUtils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/utils/FatTreeUtils.cpp

CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/utils/FatTreeUtils.cpp > CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.i

CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/utils/FatTreeUtils.cpp -o CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.s

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o: ../experiment/fat_tree/fat_tree_exp_serial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_serial.cpp

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_serial.cpp > CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.i

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_serial.cpp -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.s

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o: CMakeFiles/fat_tree_parallel.dir/flags.make
CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o: ../experiment/fat_tree/fat_tree_exp_parallel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o -c /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_parallel.cpp

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_parallel.cpp > CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.i

CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/experiment/fat_tree/fat_tree_exp_parallel.cpp -o CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.s

# Object files for target fat_tree_parallel
fat_tree_parallel_OBJECTS = \
"CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o" \
"CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o"

# External object files for target fat_tree_parallel
fat_tree_parallel_EXTERNAL_OBJECTS =

fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_parallel.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/graph/Graph.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/graph/fattree/FatTree.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/node/FatTreeNode.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/utils/FatTreeUtils.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_serial.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/experiment/fat_tree/fat_tree_exp_parallel.cpp.o
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/build.make
fat_tree_parallel: CMakeFiles/fat_tree_parallel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable fat_tree_parallel"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fat_tree_parallel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fat_tree_parallel.dir/build: fat_tree_parallel

.PHONY : CMakeFiles/fat_tree_parallel.dir/build

CMakeFiles/fat_tree_parallel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fat_tree_parallel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fat_tree_parallel.dir/clean

CMakeFiles/fat_tree_parallel.dir/depend:
	cd /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug /media/cuongbv/Data/Project/Research/InterNetwork/parallel-routing/cmake-build-debug/CMakeFiles/fat_tree_parallel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fat_tree_parallel.dir/depend


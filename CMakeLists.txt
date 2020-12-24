# Minimum Cmake verison required
cmake_minimum_required(VERSION 3.0.0)


# Project name 
project(plcruntime VERSION 0.1.0)

# Global Install Directories
include(GNUInstallDirs)

# ----------------------------------
#    Section for compiling 1 binary 
# ----------------------------------
# Compile for one binary

# List all the files for compiling one binary
set(BIN_SRC_FILES
    src/main.cpp
    src/hardware_layer.cpp 
    src/glueVars.cpp 
    src/POUS.c 
    src/STD_CONF.c
    src/STD_RESSOURCE.c)

add_executable(plcruntime ${BIN_SRC_FILES})

# Define Compiler Flags to your library
target_compile_options(plcruntime PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

# Define the Include directories 
# <BUILD_INTERFACE> for those directories that are needed for compilation.
target_include_directories(plcruntime PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

# Link to ShunyaInterfaces
target_link_libraries(plcruntime PUBLIC siCore siYaml siPeripheral siConn siCloud siIiot siDash siAlerts)
# If you have any more external libraries to link then link here 
target_link_libraries(plcruntime PUBLIC -lm -lrt -lpthread)
# ----------------------------------
#    End Section for binary
# ----------------------------------
# If you have multiple binaries in a folder 
# the add more of these sections and change binary_name

# Minimum Cmake verison required
cmake_minimum_required(VERSION 3.0.0)

# Project name 
project(plcruntime VERSION 0.1.0)

# Global Install Directories
include(GNUInstallDirs)

add_definitions(-DBUILD_GLUEGEN)

if(BUILD_GLUEGEN)
    add_subdirectory(extern/glueGen)
endif(BUILD_GLUEGEN)

# Build examples only if configured
# Turn on the Examples with cmake -DBUILD_EXAMPLES=ON ../
add_definitions(-DBUILD_RUNTIME)

if(BUILD_RUNTIME)
# ----------------------------------
#    Section for compiling 1 binary 
# ----------------------------------
# Compile for one binary

# List all the files for compiling one binary
set(BIN_SRC_FILES
    src/main.c
    src/hardware_layer.c
    src/glueVars.c 
    src/Config0.c
    src/Res0.c)

add_executable(plcruntime ${BIN_SRC_FILES})

# Define Compiler Flags to your library
target_compile_options(plcruntime PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

# Define the Include directories 
# <BUILD_INTERFACE> for those directories that are needed for compilation.
target_include_directories(plcruntime PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/matiec_src/lib/C/>    
    )

# Link to ShunyaInterfaces
target_link_libraries(plcruntime PUBLIC siCore siYaml siPeripheral siConn siCloud siIiot siDash siAlerts siFunctions)
# If you have any more external libraries to link then link here 
target_link_libraries(plcruntime PUBLIC -lm -lrt -lpthread)
endif(BUILD_RUNTIME)

add_custom_command(OUTPUT matiec.txt
    COMMAND  autoreconf -i || exit 0
    COMMAND  ./configure || exit 0
    COMMAND  make -j4 || exit 0
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/matiec_src"
    COMMENT "Compiling matiec"
    VERBATIM)

add_custom_target(matiec DEPENDS matiec.txt)

add_custom_command(OUTPUT genC.txt
    COMMAND  ./iec2c ../plc_program/main.st || exit 0
    COMMAND  cp -dpRv POUS.c POUS.h Config0.c Config0.h Res0.c LOCATED_VARIABLES.h VARIABLES.csv ../../src/ || exit 0
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern/matiec_src"
    COMMENT "Generating the C files from extern/plc_program/main.st"
    VERBATIM)

add_custom_target(iec2c DEPENDS matiec.txt genC.txt)

add_custom_command(OUTPUT glueGen.txt
    COMMAND  ./bin/glueGen ./src/LOCATED_VARIABLES.h ./src/glueVars.c || exit 0
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating the glue layer from src/LOCATED_VARIABLES.h"
    VERBATIM)

add_custom_target(genglue DEPENDS glueGen.txt)


# ----------------------------------
#    End Section for binary
# ----------------------------------
# If you have multiple binaries in a folder 
# the add more of these sections and change binary_name

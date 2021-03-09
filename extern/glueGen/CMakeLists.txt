cmake_minimum_required(VERSION 3.0.0)

project(runtime_gluegenerator)

# Global Install Directories
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# The main application that we always build
add_executable(glueGen glueGen.cpp)
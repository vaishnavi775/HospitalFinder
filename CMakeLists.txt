# Minimum required version of CMake
cmake_minimum_required(VERSION 3.0)

# Project name
project(Project)

# Source Files
set(SOURCE_FILES
    src/main.c
    src/colleges.txt
    src/comments.txt
    src/hospitals.txt
    src/info.txt
    src/outputFile.txt
    src/reviews.txt
)


include_directories(${CMAKE_SOURCE_DIR}/include)

# Add an executable
add_executable(my_project ${SOURCE_FILES})

# Link the executable with the math library
target_link_libraries(my_project m)

# Specify where to put the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Set a custom output directory for build files
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Install the executable
install(TARGETS my_project DESTINATION ${CMAKE_SOURCE_DIR}/bin)


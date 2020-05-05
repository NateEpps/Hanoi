cmake_minimum_required(VERSION 3.0)

project(HanoiTowers)

# Find SFML
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules/)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# Grab Source
add_subdirectory(src)
message(STATUS "Source Files: ${Source}")

# Debug?
option(DEBUG "Build debug mode?" YES)
if(DEBUG)
	message(STATUS "Building Debug mode")
	add_definitions(-DDEBUG -g)
else()
	message(STATUS "Building Release mode")
endif()

# Build
add_definitions(-std=c++1y -O2)

include_directories(${SFML_INCLUDE_DIR})

add_executable(Hanoi ${Source})

# Link
target_link_libraries(Hanoi ${SFML_LIBRARIES})

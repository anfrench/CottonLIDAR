cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(GroundRemoval)

find_package(PCL 1.7 REQUIRED)

include_directories( ${PCL_INCLUDE_DIRS})
link_directories( ${PCL_LIBRARY_DIRS})
add_definitions( ${PCL_DEFINITIONS})

add_executable (GroundRemoval GroundRemoval.cpp)
target_link_libraries (GroundRemoval ${PCL_LIBRARIES})

#target_link_libraries (GroundRemoval)

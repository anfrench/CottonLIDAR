cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(BollCount)

find_package(PCL 1.7 REQUIRED)

include_directories( ${PCL_INCLUDE_DIRS})
link_directories( ${PCL_LIBRARY_DIRS})
add_definitions( ${PCL_DEFINITIONS})

add_executable (BollCount BollCount.cpp)
target_link_libraries (BollCount ${PCL_LIBRARIES} )

#target_link_libraries (BollCount)

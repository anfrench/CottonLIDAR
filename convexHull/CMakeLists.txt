cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ConvexHull)

find_package(PCL 1.7 REQUIRED)

include_directories( ${PCL_INCLUDE_DIRS})
link_directories( ${PCL_LIBRARY_DIRS})
add_definitions( ${PCL_DEFINITIONS})

add_executable (ConvexHull ConvexHull.cpp)
target_link_libraries (ConvexHull ${PCL_LIBRARIES} )

#target_link_libraries (ConvexHull)

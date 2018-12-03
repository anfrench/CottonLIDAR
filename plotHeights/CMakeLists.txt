cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(f119PlotHeight)

find_package(PCL 1.7 REQUIRED)

include_directories( ${PCL_INCLUDE_DIRS})
link_directories( ${PCL_LIBRARY_DIRS})
add_definitions( ${PCL_DEFINITIONS})

add_executable (f119PlotHeight f119PlotHeight.cpp)
target_link_libraries (f119PlotHeight ${PCL_LIBRARIES} )

#target_link_libraries (f119PlotHeight)

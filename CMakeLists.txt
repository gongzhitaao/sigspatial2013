cmake_minimum_required (VERSION 2.8)

project (SigSpatial2013)

enable_testing ()

set (CMAKE_CXX_FLAGS "-frounding-math -Wall -std=c++11 -O3")
set (CMAKE_CXX_FLAGS_DEBUG "-frounding-math -Wall -std=c++11 -g")
set (CMAKE_CXX_FLAGS_RELEASE "-frounding-math -Wall -std=c++11 -O3")

include_directories (inc)

add_subdirectory (src)
add_subdirectory (test)

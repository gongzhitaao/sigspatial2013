set (CMAKE_CXX_FLAGS "-frounding-math -Wall -std=c++11 -O3")
set (CMAKE_CXX_FLAGS_DEBUG "-frounding-math -Wall -std=c++11 -g")
set (CMAKE_CXX_FLAGS_RELEASE "-frounding-math -Wall -std=c++11 -O3")

add_library (gmlparser STATIC gmlparser.cpp gmlparser.h asm-xml.o)
target_link_libraries (gmlparser)

add_library (grid STATIC grid.cpp grid.h)

add_library (polygon STATIC polygon.cpp polygon.h)
target_link_libraries (polygon grid)

add_library (core STATIC inside.cpp within.cpp)
target_link_libraries (core polygon gmlparser CGAL CGAL_Core tbb pthread)

add_executable (geofence main.cpp)
target_link_libraries (geofence core)

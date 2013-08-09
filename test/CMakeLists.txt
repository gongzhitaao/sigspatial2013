find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS})

add_executable (inside inside.cpp)
target_link_libraries (inside core ${GTEST_BOTH_LIBRARIES})

add_executable (within within.cpp)
target_link_libraries (within core ${GTEST_BOTH_LIBRARIES})

add_test (inside inside)
add_test (within within)

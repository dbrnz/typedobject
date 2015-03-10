cmake_minimum_required(VERSION 3.0)

project("Abstract Objects in C++")

# We use C++11
if (UNIX)
  add_definitions(-std=c++11)
endif()

SET(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR})
find_package(typedobject)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

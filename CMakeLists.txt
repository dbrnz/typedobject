cmake_minimum_required(VERSION 2.8)

project("Abstract Objects in C++")




# We use C++11

if (UNIX)
  add_definitions(-std=c++11)
endif()



enable_testing()

add_subdirectory(src)

add_subdirectory(tests)

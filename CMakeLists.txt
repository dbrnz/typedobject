cmake_minimum_required(VERSION 3.0)

project("Abstract Objects in C++")

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  add_definitions(-DRAPTOR_STATIC)
endif()

SET(typedobject_DIR ${CMAKE_CURRENT_LIST_DIR})
include(${typedobject_DIR}/cmake/typedobject_generate.cmake)
include(GenerateExportHeader)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
add_library(typedobject SHARED ${SOURCES})

target_include_directories(typedobject PUBLIC ${INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(typedobject ${LIBS})

generate_export_header(typedobject)


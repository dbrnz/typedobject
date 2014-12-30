set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(rdf)

set(SOURCES ${SOURCES} bsml.cpp)
set(SOURCES ${SOURCES} recording.cpp aoc_recording.cpp)

set(SOURCES ${SOURCES} aobject.cpp aoc_aobject.cpp)
set(SOURCES ${SOURCES} utils.cpp)

add_library(aobject SHARED ${SOURCES})

target_include_directories(aobject PUBLIC ${INCLUDE_DIRS})
target_link_libraries(aobject ${LIBRARIES})

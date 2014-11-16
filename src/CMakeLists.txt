set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(rdf)

add_library(aobject SHARED ${SOURCES})

target_include_directories(aobject PUBLIC ${INCLUDE_DIRS})
target_link_libraries(aobject ${LIBRARIES})

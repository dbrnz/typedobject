include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_rdf rdf.cpp)
target_link_libraries(test_rdf typedobject)
add_test(RDF, test_rdf)

add_executable(test_utils utils.cpp)
target_link_libraries(test_utils typedobject)
add_test(UTILS, test_utils)

include(${CMAKE_SOURCE_DIR}/cmake/typedobject_generate.cmake)
typedobject_generate(GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/types.h "${INCLUDES}")
add_executable(test_types types.cpp ${GENERATED_FILES})
target_link_libraries(test_types typedobject)
add_test(TYPES, test_types)

typedobject_generate(GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/example.h "${INCLUDES}")
add_executable(test_example example.cpp ${GENERATED_FILES})
target_link_libraries(test_example typedobject)
add_test(EXAMPLE, test_example)

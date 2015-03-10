include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_rdf rdf.cpp)
target_link_libraries(test_rdf typedobject)
add_test(RDF, test_rdf)

typedobject_generate(GENERATED_FILES types.h)
add_executable(test_types types.cpp ${GENERATED_FILES})
target_link_libraries(test_types typedobject)
add_test(TYPES, test_types)


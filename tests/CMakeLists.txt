add_executable(test_rdf rdf.cpp)
target_link_libraries(test_rdf typedobject)
add_test(RDF, test_rdf)

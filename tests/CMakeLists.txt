add_executable(test_rdf rdf.cpp)

target_link_libraries(test_rdf aobject)


add_test(RDF, test_rdf)

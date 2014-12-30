add_executable(test_rdf rdf.cpp)
target_link_libraries(test_rdf aobject)
add_test(RDF, test_rdf)

add_executable(test_bsml bsml.cpp)
target_link_libraries(test_bsml aobject)
add_test(BSML, test_bsml)


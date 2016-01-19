include_directories(${INCLUDES})

### Find Boost unit testing package:
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_BOOST_CMAKE         ON)
find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_definitions (-DBOOST_TEST_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS})
###

set(LIBRARIES typedobject
              ${Boost_FILESYSTEM_LIBRARY}
              ${Boost_SYSTEM_LIBRARY}
              ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
              )

add_executable(test_rdf rdf.cpp)
target_link_libraries(test_rdf typedobject)
add_test(RDF, test_rdf)

add_executable(test_graph graph.cpp)
target_link_libraries(test_graph typedobject)
add_test(GRAPH, test_graph)

add_executable(test_utils utils.cpp)
target_link_libraries(test_utils typedobject)
add_test(UTILS, test_utils)

include(${CMAKE_SOURCE_DIR}/cmake/typedobject_generate.cmake)
typedobject_generate(GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/types.h "${INCLUDES}")
add_executable(test_types types.cpp ${GENERATED_FILES})
target_link_libraries(test_types typedobject)
add_test(TYPES, test_types)

set(GENERATED_FILES "")
typedobject_generate(GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/example.h "${INCLUDES}")
add_executable(test_example example.cpp ${GENERATED_FILES})
target_link_libraries(test_example typedobject)
add_test(EXAMPLE, test_example)

add_executable(unittest unittest.cpp)
target_link_libraries(unittest ${LIBRARIES})
add_test(UNITTEST, unittest)

add_executable(test_units units.cpp)
target_link_libraries(test_units typedobject)
add_test(UNITS, test_units)

add_executable(test_xsd xsd.cpp)
target_link_libraries(test_xsd typedobject)
add_test(XSD, test_xsd)

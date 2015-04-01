# Find Boost date time package:
#
find_package(Boost COMPONENTS date_time)
set(INCLUDES ${Boost_INCLUDE_DIRS})
set(LIBS ${Boost_DATE_TIME_LIBRARY})  ## Only if Boost datetime IO


# Use pkg-config to find RDF libraries:
#
find_package(PkgConfig)


# Find sord and its dependencies:
#
pkg_check_modules(SORD QUIET sord-0)

set(INCLUDES ${INCLUDES} ${SORD_INCLUDE_DIRS})
add_definitions(${SORD_CFLAGS_OTHER})

set(LIBNUM 1)
foreach(LIBRARY_NAME ${SORD_LIBRARIES})
  find_library(LIBRARY${LIBNUM} ${LIBRARY_NAME} PATHS ${SORD_LIBRARY_DIRS})
  set(LIBS ${LIBS} ${LIBRARY${LIBNUM}})
  MATH(EXPR LIBNUM "${LIBNUM}+1")
endforeach()


# Find raptor:
#
pkg_check_modules(RAPTOR QUIET raptor2)

set(INCLUDES ${INCLUDES} ${RAPTOR_INCLUDE_DIRS})
add_definitions(${RAPTOR_CFLAGS_OTHER})

find_library(LIBRARY${LIBNUM} ${RAPTOR_LIBRARIES} PATHS ${RAPTOR_LIBRARY_DIRS})
set(LIBS ${LIBS} ${LIBRARY${LIBNUM}})

#

set(SOURCES typedobject.cpp
            rdf.cpp
            rdfdefs.cpp
            utils.cpp
            )

add_library(typedobject SHARED ${SOURCES})

target_include_directories(typedobject PUBLIC ${INCLUDES})
target_link_libraries(typedobject ${LIBS})

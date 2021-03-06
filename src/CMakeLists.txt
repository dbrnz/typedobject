### Find Boost date time package:
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_BOOST_CMAKE         ON)
find_package(Boost COMPONENTS date_time)

if(Boost_FOUND)
  set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS})
  set(LIBS ${LIBS} ${Boost_LIBRARIES})
  message("Boost libraries: ${Boost_LIBRARIES}")
else()
  message("ERROR: Can't find Boost libraries")
endif()
###


### Find sord and its dependencies:
pkg_check_modules(SORD REQUIRED sord-0)

set(INCLUDES ${INCLUDES} ${SORD_INCLUDE_DIRS})
add_definitions(${SORD_CFLAGS_OTHER})

message("Sord: ${SORD_STATIC_LIBRARIES}")
foreach(LIBRARY_NAME ${SORD_STATIC_LIBRARIES})
  unset(LIBNAME CACHE)
  find_library(LIBNAME ${LIBRARY_NAME} PATHS ${SORD_STATIC_LIBRARY_DIRS})
  if(LIBNAME)
    set(LIBS ${LIBS} ${LIBNAME})
  else()
    set(LIBS ${LIBS} ${LIBRARY_NAME})
  endif()
endforeach()
###


### Find Raptor and its dependencies:
pkg_check_modules(RAPTOR REQUIRED raptor2)

set(INCLUDES ${INCLUDES} ${RAPTOR_INCLUDE_DIRS})
add_definitions(${RAPTOR_CFLAGS_OTHER})

message("Raptor: ${RAPTOR_STATIC_LIBRARIES}")
foreach(LIBRARY_NAME ${RAPTOR_STATIC_LIBRARIES})
  unset(LIBNAME CACHE)
  find_library(LIBNAME ${LIBRARY_NAME} PATHS ${RAPTOR_STATIC_LIBRARY_DIRS})
  if(LIBNAME)
    set(LIBS ${LIBS} ${LIBNAME})
  else()
    set(LIBS ${LIBS} ${LIBRARY_NAME})
  endif()
endforeach()
###


set(INCLUDES ${INCLUDES} PARENT_SCOPE)
set(LIBS     ${LIBS}     PARENT_SCOPE)

set(SOURCES ${SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/typedobject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rdf.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rdfimpl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/xsd.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/xsdimpl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/units.cpp
    PARENT_SCOPE
    )

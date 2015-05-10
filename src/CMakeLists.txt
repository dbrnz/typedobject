### Find Boost date time package:
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
if(WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()
find_package(Boost COMPONENTS date_time)

if(Boost_FOUND)
  set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS})
  set(LIBS ${LIBS} ${Boost_LIBRARIES})
else()
  message(ERROR "Can't find Boost date time library")
endif()
###


### Find sord and its dependencies:
pkg_check_modules(SORD REQUIRED sord-0)

set(INCLUDES ${INCLUDES} ${SORD_INCLUDE_DIRS})
add_definitions(${SORD_CFLAGS_OTHER})

foreach(LIBRARY_NAME ${SORD_LIBRARIES})
  unset(LIBNAME CACHE)
  find_library(LIBNAME ${LIBRARY_NAME} PATHS ${SORD_LIBRARY_DIRS})
  set(LIBS ${LIBS} ${LIBNAME})
endforeach()
###


### Find Raptor and its dependencies:
pkg_check_modules(RAPTOR REQUIRED raptor2)

set(INCLUDES ${INCLUDES} ${RAPTOR_INCLUDE_DIRS})
add_definitions(${RAPTOR_CFLAGS_OTHER})

foreach(LIBRARY_NAME ${RAPTOR_LIBRARIES})
  unset(LIBNAME CACHE)
  find_library(LIBNAME ${LIBRARY_NAME} PATHS ${RAPTOR_LIBRARY_DIRS})
  set(LIBS ${LIBS} ${LIBNAME})
endforeach()
###


set(INCLUDES ${INCLUDES} PARENT_SCOPE)
set(LIBS     ${LIBS}     PARENT_SCOPE)

set(SOURCES ${SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/typedobject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rdf.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rdfimpl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rdfdefs.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    PARENT_SCOPE
    )

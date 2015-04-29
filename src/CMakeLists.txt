# Find Boost date time package:
#
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS date_time)

if(Boost_FOUND)
  set(INCLUDES ${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
  set(LIBS ${Boost_DATE_TIME_LIBRARY})
else()
  message(ERROR "Can't find Boost date time library")
endif()



# Use pkg-config to find RDF libraries:
#
find_package(PkgConfig)

# Find sord and its dependencies:
#
pkg_check_modules(SORD REQUIRED sord-0)

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
pkg_check_modules(RAPTOR REQUIRED raptor2)

set(INCLUDES ${INCLUDES} ${RAPTOR_INCLUDE_DIRS})
add_definitions(${RAPTOR_CFLAGS_OTHER})

find_library(LIBRARY${LIBNUM} ${RAPTOR_LIBRARIES} PATHS ${RAPTOR_LIBRARY_DIRS})
set(LIBS ${LIBS} ${LIBRARY${LIBNUM}})

#
if(WIN32)
  add_definitions(-DRAPTOR_STATIC)

  SET(LIBS ${LIBS}
           c:/utils/lib/libxml2.lib
           c:/utils/lib/libxslt.lib
           c:/utils/lib/iconv.lib
           c:/utils/lib/zlib.lib
      )
  message("LIBS: " ${LIBS})
endif()

add_definitions(-DTYPEDOBJECT_BUILD)

add_definitions(-DTYPEDOBJECT_STATIC)

set(SOURCES typedobject.cpp
            rdf.cpp
            rdfdefs.cpp
            utils.cpp
            )

add_library(typedobject SHARED ${SOURCES})

target_include_directories(typedobject PUBLIC ${INCLUDES})
target_link_libraries(typedobject ${LIBS})

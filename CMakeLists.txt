cmake_minimum_required(VERSION 3.0)

project("Abstract Objects in C++")
set(TypedObject_VERSION 0.0.5)

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  add_definitions(-DRAPTOR_STATIC)
endif()

SET(typedobject_DIR ${CMAKE_CURRENT_LIST_DIR})
include(${typedobject_DIR}/cmake/typedobject_generate.cmake)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)


add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
add_library(typedobject SHARED ${SOURCES})

target_include_directories(typedobject PUBLIC ${INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(typedobject ${LIBS})

generate_export_header(typedobject)

set_property(TARGET typedobject PROPERTY VERSION ${TypedObject_VERSION})
set_property(TARGET typedobject PROPERTY SOVERSION 3)
set_property(TARGET typedobject PROPERTY INTERFACE_typedobject_MAJOR_VERSION 0)
set_property(TARGET typedobject APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING typedobject_MAJOR_VERSION
  )

install(TARGETS typedobject EXPORT typedobjectTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )

install(
  FILES
    src/typedobject.h
    src/rdf.h
    src/rdfdefs.h
    src/utils.h
    "${CMAKE_CURRENT_BINARY_DIR}/typedobject_export.h"
  DESTINATION
    include/typedobject
  COMPONENT
    Devel
  )


write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/typedobject/typedobjectConfigVersion.cmake"
  VERSION ${TypedObject_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

export(EXPORT typedobjectTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/typedobject/typedobjectTargets.cmake"
  NAMESPACE TypedObject::
  )
configure_file(cmake/typedobjectConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/typedobject/typedobjectConfig.cmake"
  COPYONLY
  )

if(WIN32)
  set(ConfigPackageLocation cmake)
else()
  set(ConfigPackageLocation lib/cmake/typedobject)
endif()

install(EXPORT typedobjectTargets
  FILE
    typedobjectTargets.cmake
  NAMESPACE
    TypedObject::
  DESTINATION
    ${ConfigPackageLocation}
  )

install(
  FILES
    cmake/typedobjectConfig.cmake
    cmake/typedobject_generate.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/typedobject/typedobjectConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
  )

install(
  FILES
    bin/generate.py
  DESTINATION
    ${ConfigPackageLocation}/bin
  COMPONENT
    Devel
  )

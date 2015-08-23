cmake_minimum_required(VERSION 3.0)

project("TypedObjectLib")
set(TypedObject_VERSION 1.0.2)

### Build a package...
if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
include(CPack)

### CMake helpers...
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

### Use pkg-config to find libraries:
find_package(PkgConfig)

set(typedobject_DIR ${CMAKE_SOURCE_DIR})

if (UNIX)
  add_definitions(-std=c++11)  # Use C++11
  add_definitions(-Wall -Wextra -Wstrict-aliasing -Wstrict-overflow)
  add_definitions(-Wno-unused-parameter)
elseif(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-DRAPTOR_STATIC)
endif()

set(INCLUDES $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

add_library(typedobject SHARED ${SOURCES})
target_include_directories(typedobject PUBLIC ${INCLUDES})
target_link_libraries(typedobject ${LIBS})

set_property(TARGET typedobject PROPERTY VERSION ${TypedObject_VERSION})
set_property(TARGET typedobject PROPERTY SOVERSION 1)
set_property(TARGET typedobject PROPERTY INTERFACE_typedobject_MAJOR_VERSION 1)
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
  DIRECTORY
    ${CMAKE_SOURCE_DIR}/include/typedobject
  DESTINATION
    include
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

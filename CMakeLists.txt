cmake_minimum_required(VERSION 3.0)

project("TypedObjectLib")
set(TypedObject_VERSION_MAJOR 1)
set(TypedObject_VERSION_MINOR 1)

set(TypedObject_VERSION ${TypedObject_VERSION_MAJOR}.${TypedObject_VERSION_MINOR})
set(TypedObject_VERSION_SO ${TypedObject_VERSION_MAJOR})

### CMake helpers...
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

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Building DEBUG version")
else()
  message(STATUS "Building RELEASE version")
endif()

set(INCLUDES $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

add_library(typedobject SHARED ${SOURCES})
target_include_directories(typedobject PUBLIC ${INCLUDES})
target_link_libraries(typedobject ${LIBS})

set_property(TARGET typedobject PROPERTY VERSION ${TypedObject_VERSION})
set_property(TARGET typedobject PROPERTY SOVERSION ${TypedObject_VERSION_SO})
set_property(TARGET typedobject PROPERTY INTERFACE_typedobject_VERSION_MAJOR ${TypedObject_VERSION_MAJOR})
set_property(TARGET typedobject APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING typedobject_VERSION_MAJOR
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
  @ONLY
  )

# Where package configuration files are put
set(ConfigPackageLocation lib/cmake/typedobject)

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
    cmake/typedobject_generate.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/typedobject/typedobjectConfig.cmake"
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

# Ensure symbols are stripped from packaged libraries
install(SCRIPT cmake/strip_libraries.cmake)

### Build a package...
if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR ${TypedObject_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${TypedObject_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION ${TypedObject_VERSION})
include(CPack)

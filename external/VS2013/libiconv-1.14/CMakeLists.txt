cmake_minimum_required(VERSION 3.0)

project("libiconv")

add_definitions(-DPACKAGE_VERSION_MAJOR=1
                -DPACKAGE_VERSION_MINOR=14
                -DPACKAGE_VERSION_SUBMINOR=0
                -DPACKAGE_VERSION_STRING="1.14.0"
                )

if(WIN32)
  set(ICONV_CONST     const)
  set(USE_MBSTATE_T   USE_MBSTATE_T)
  set(HAVE_WCHAR_T    HAVE_WCHAR_T)
  set(BROKEN_WCHAR_H  BROKEN_WCHAR_H)
  set(HAVE_VISIBILITY 1)
endif()

CONFIGURE_FILE(
	libcharset/include/localcharset.h.build.in
	include/iconv/localcharset.h
	@ONLY
  NEWLINE_STYLE UNIX
  )

CONFIGURE_FILE(
  include/iconv.h.build.in
  include/iconv/iconv.h
	@ONLY
  NEWLINE_STYLE UNIX
  )

CONFIGURE_FILE(
  config.h.cmake.in
  include/iconv/config.h
	@ONLY
  NEWLINE_STYLE UNIX
  )

CONFIGURE_FILE(
  pkg-config.pc.cmake
  iconv.pc
  NEWLINE_STYLE UNIX
  )

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_BINARY_DIR}/include/iconv)

set(SOURCES lib/iconv.c
            lib/relocatable.c
            libcharset/lib/localcharset.c
            )

if(WIN32)
  set(RESOURCES windows/libiconv.rc)
endif()

add_library(libiconv STATIC ${SOURCES} ${RESOURCES})
set_target_properties(libiconv PROPERTIES DEBUG_POSTFIX "d")

install(TARGETS libiconv
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install(
  DIRECTORY
    ${CMAKE_BINARY_DIR}/include/iconv
  DESTINATION
    include
  COMPONENT
    Devel
  )

install(
  FILES
    ${CMAKE_BINARY_DIR}/iconv.pc
  DESTINATION
    share/pkgconfig
  COMPONENT
    Devel
  )

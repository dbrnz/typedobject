set(INCLUDE_DIRS ${INCLUDE_DIRS}
  /usr/local/include/serd-0  ## From pkg_config ??
  /usr/local/include/sord-0  ## From pkg_config ??
  PARENT_SCOPE)

LINK_DIRECTORIES(/usr/local/lib)

set(LIBRARIES ${LIBRARIES}
  serd-0
  sord-0
  PARENT_SCOPE)

set(SOURCES ${SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/rdf.cpp
  PARENT_SCOPE)


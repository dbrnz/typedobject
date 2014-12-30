function(make_aoc output filename)
  get_filename_component(base ${filename} NAME_WE)
  set(aoc_file ${CMAKE_CURRENT_BINARY_DIR}/aoc_${base}.cpp)
  set(${output} ${${output}} ${aoc_file} PARENT_SCOPE)
  add_custom_command(
    OUTPUT ${aoc_file}
    COMMAND python ${CMAKE_SOURCE_DIR}/src/clang/aoc.py
                   ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
                   ${aoc_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
    )
  set_source_files_properties(${aoc_file} PROPERTIES GENERATED TRUE)
endfunction()


set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(rdf)

set(SOURCES ${SOURCES} bsml.cpp)
set(SOURCES ${SOURCES} aobject.cpp recording.cpp)
set(SOURCES ${SOURCES} utils.cpp)

set(HEADERS aobject.h recording.h)
foreach(file ${HEADERS})
  make_aoc(GENERATED_AOC ${file})
endforeach()

source_group("" FILES ${SOURCES})
source_group(aoc FILES ${GENERATED_AOC})
add_library(aobject SHARED ${SOURCES} ${GENERATED_AOC})

target_include_directories(aobject PUBLIC ${INCLUDE_DIRS})
target_link_libraries(aobject ${LIBRARIES})

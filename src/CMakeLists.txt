function(make_aoc output filename)
  get_filename_component(base ${filename} NAME_WE)
  set(tobj_file ${CMAKE_CURRENT_BINARY_DIR}/tobj_${base}.cpp)
  set(${output} ${${output}} ${tobj_file} PARENT_SCOPE)
  add_custom_command(
    OUTPUT ${tobj_file}
    COMMAND python ${CMAKE_SOURCE_DIR}/src/clang/aoc.py
                   ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
                   ${tobj_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
    )
  set_source_files_properties(${tobj_file} PROPERTIES GENERATED TRUE)
endfunction()


find_package(Boost COMPONENTS date_time)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${Boost_DATE_TIME_LIBRARY})  ## Only if Boost datetime IO
message(${LIBRARIES})  ## Only if Boost datetime IO

add_subdirectory(rdf)

set(SOURCES ${SOURCES} ontology.cpp)
set(SOURCES ${SOURCES} aobject.cpp)
set(SOURCES ${SOURCES} object.cpp recording.cpp signal.cpp segment.cpp
                       timing.cpp event.cpp annotation.cpp)
set(SOURCES ${SOURCES} utils.cpp)

set(HEADERS ## aobject.h
            object.h recording.h signal.h segment.h timing.h event.h annotation.h)
foreach(file ${HEADERS})
  make_aoc(GENERATED_AOC ${file})
endforeach()

source_group("" FILES ${SOURCES})
source_group(aoc FILES ${GENERATED_AOC})
add_library(aobject SHARED ${SOURCES} ${GENERATED_AOC})

target_include_directories(aobject PUBLIC ${INCLUDE_DIRS})
target_link_libraries(aobject ${LIBRARIES})

function(typedobject_generate generated_files header_file)

  string(LENGTH ${CMAKE_SOURCE_DIR} srclen)
  get_filename_component(dir ${header_file} DIRECTORY)
  if("${dir}" STREQUAL "")
    set(dir ${CMAKE_SOURCE_DIR})
  endif()
  string(SUBSTRING ${dir} ${srclen} -1 dir)

  get_filename_component(base ${header_file} NAME_WE)

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${dir})
  set(gen_file ${CMAKE_BINARY_DIR}/${dir}/tobj_${base}.cpp)
  set(${generated_files} ${${generated_files}} ${gen_file} PARENT_SCOPE)

  add_custom_command(
    OUTPUT ${gen_file}
    COMMAND python ${typedobject_DIR}/src/clang/generate.py
                   -I${CMAKE_SOURCE_DIR}/src
                   -I${typedobject_DIR}/include
                   ${CMAKE_CURRENT_SOURCE_DIR}
                   ${header_file}
                   ${gen_file}
    DEPENDS ${header_file}
    )
  set_source_files_properties(${gen_file} PROPERTIES GENERATED TRUE)
endfunction()

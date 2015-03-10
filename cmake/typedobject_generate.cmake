function(typedobject_generate generated_files header_file)
  get_filename_component(base ${header_file} NAME_WE)
  set(gen_file ${CMAKE_CURRENT_BINARY_DIR}/tobj_${base}.cpp)
  set(${generated_files} ${${generated_files}} ${gen_file} PARENT_SCOPE)
  add_custom_command(
    OUTPUT ${gen_file}
    COMMAND python ${typedobject_DIR}/src/clang/generate.py
                   -I${typedobject_DIR}/src
                   ${CMAKE_CURRENT_SOURCE_DIR}/${header_file}
                   ${gen_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${header_file}
    )
  set_source_files_properties(${gen_file} PROPERTIES GENERATED TRUE)
endfunction()

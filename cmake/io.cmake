function(
  exclude_sources_by_regex
  src_files
  regex
)
  foreach(PATH ${${src_files}})
    if(${PATH} MATCHES ${regex})
      message(STATUS "Removed: '${PATH}'")
      list(REMOVE_ITEM ${src_files} ${PATH})
    else()
      message(STATUS "Keep: '${PATH}'")
    endif()
  endforeach()

  return(
    PROPAGATE
    ${src_files}
  )
endfunction()

add_library(utils STATIC
  string.cc
  utils.cc
)

if(NOT ANDROID)
  if(MSVC)
    target_link_libraries(utils)
  else()
    target_link_libraries(utils PUBLIC dl)
  endif()
endif()
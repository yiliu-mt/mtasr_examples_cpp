add_library(utils STATIC
  string.cc
  utils.cc
)

if(NOT ANDROID)
  if(MSVC)
    target_link_libraries(utils PUBLIC glog gflags_nothreads)
  else()
    target_link_libraries(utils PUBLIC glog gflags_nothreads dl)
  endif()
endif()
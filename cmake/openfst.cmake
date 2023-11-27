include(gflags)
include(glog)

set(CONFIG_FLAGS "")
if(NOT FST_HAVE_BIN)
  if(MSVC)
    set(HAVE_BIN OFF CACHE BOOL "Build the fst binaries" FORCE)
  else()
    set(CONFIG_FLAGS "--disable-bin")
  endif()
endif()

set(openfst_SOURCE_DIR ${fc_base}/openfst-src CACHE PATH "OpenFST source directory")
set(openfst_BINARY_DIR ${fc_base}/openfst-build CACHE PATH "OpenFST build directory")
set(openfst_PREFIX_DIR ${fc_base}/openfst-subbuild/openfst-populate-prefix CACHE PATH "OpenFST prefix directory")
if(NOT MSVC)
  ExternalProject_Add(openfst
    URL               https://github.com/mjansche/openfst/archive/1.6.5.zip
    URL_HASH          SHA256=b720357a464f42e181d7e33f60867b54044007f50baedc8f4458a3926f4a5a78
    PREFIX            ${openfst_PREFIX_DIR}
    SOURCE_DIR        ${openfst_SOURCE_DIR}
    BINARY_DIR        ${openfst_BINARY_DIR}
    CONFIGURE_COMMAND ${openfst_SOURCE_DIR}/configure ${CONFIG_FLAGS} --prefix=${openfst_PREFIX_DIR}
                        "CPPFLAGS=-I${gflags_BINARY_DIR}/include -I${glog_SOURCE_DIR}/src -I${glog_BINARY_DIR} ${TORCH_CXX_FLAGS}"
                        "LDFLAGS=-L${gflags_BINARY_DIR} -L${glog_BINARY_DIR}"
                        "LIBS=-lgflags_nothreads -lglog -lpthread"
    COMMAND           ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/patch/openfst ${openfst_SOURCE_DIR}
    BUILD_COMMAND     make -j$(nproc)
  )
  add_dependencies(openfst gflags glog)
  link_directories(${openfst_PREFIX_DIR}/lib)
else()
  add_compile_options(/W0 /wd4244 /wd4267)
  FetchContent_Declare(openfst
    URL           https://github.com/kkm000/openfst/archive/refs/tags/win/1.6.5.1.tar.gz
    URL_HASH      SHA256=02c49b559c3976a536876063369efc0e41ab374be1035918036474343877046e
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/patch/openfst ${openfst_SOURCE_DIR}
  )
  FetchContent_MakeAvailable(openfst)
  add_dependencies(fst gflags glog)
  target_link_libraries(fst PUBLIC gflags_nothreads_static glog)
endif()
include_directories(${openfst_SOURCE_DIR}/src/include)

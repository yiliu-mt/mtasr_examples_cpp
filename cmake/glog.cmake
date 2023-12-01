FetchContent_Declare(glog
  URL      https://mt-ai-speech-public.tos-cn-beijing.volces.com/glog-0.4.0.zip
  URL_HASH SHA256=9e1b54eb2782f53cd8af107ecf08d2ab64b8d0dc2b7f5594472f3bd63ca85cdc
)
FetchContent_MakeAvailable(glog)
include_directories(${glog_SOURCE_DIR}/src ${glog_BINARY_DIR})
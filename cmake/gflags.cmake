FetchContent_Declare(gflags
  URL      https://mt-ai-speech-public.tos-cn-beijing.volces.com/gflags-2.2.2.zip
  URL_HASH SHA256=19713a36c9f32b33df59d1c79b4958434cb005b5b47dc5400a7a4b078111d9b5
)
FetchContent_MakeAvailable(gflags)
include_directories(${gflags_BINARY_DIR}/include)
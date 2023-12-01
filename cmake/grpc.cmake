include_directories(${CMAKE_CURRENT_SOURCE_DIR}/grpc)
# third_party: grpc
# On how to build grpc, you may refer to https://github.com/grpc/grpc
# We recommend manually recursive clone the repo to avoid internet connection problem
FetchContent_Declare(gRPC
  URL           https://mt-ai-speech-public.tos-cn-beijing.volces.com/grpc_asr.tar.gz
  URL_HASH      SHA256=0888d160f688b869c667f09caac86fc18f893bc44a6892652bf667f2f4e7723a
)
FetchContent_MakeAvailable(gRPC)
# compile asr.proto
set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
add_custom_command(
  OUTPUT  ${PROTO_DIR}/asr.pb.cc
          ${PROTO_DIR}/asr.pb.h
          ${PROTO_DIR}/asr.grpc.pb.cc
          ${PROTO_DIR}/asr.grpc.pb.h
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../lib/grpc/third_party/protobuf/src/protoc
  ARGS --grpc_out "${PROTO_DIR}"
    --cpp_out "${PROTO_DIR}"
    -I "${PROTO_DIR}"
    --plugin=protoc-gen-grpc=${CMAKE_CURRENT_SOURCE_DIR}/../lib/grpc/bins/opt/grpc_cpp_plugin
    asr.proto)

# grpc_client
add_library(asr_grpc STATIC
  grpc_client.cc
  asr.pb.cc
  asr.grpc.pb.cc
)
target_link_libraries(asr_grpc PUBLIC grpc++ grpc++_reflection utils)

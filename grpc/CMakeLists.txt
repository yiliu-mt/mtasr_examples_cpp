if (NOT MSVC)
  # compile asr.proto
  set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  add_custom_command(
    OUTPUT  ${PROTO_DIR}/asr.pb.cc
            ${PROTO_DIR}/asr.pb.h
            ${PROTO_DIR}/asr.grpc.pb.cc
            ${PROTO_DIR}/asr.grpc.pb.h
    COMMAND ${protobuf_BINARY_DIR}/protoc
    ARGS --grpc_out "${PROTO_DIR}"
      --cpp_out "${PROTO_DIR}"
      -I "${PROTO_DIR}"
      --plugin=protoc-gen-grpc=${grpc_BINARY_DIR}/grpc_cpp_plugin
      asr.proto)
endif()

# grpc_client
if (NOT MSVC)
  link_directories(${protobuf_BINARY_DIR}/lib)
endif()
add_library(asr_grpc STATIC
  grpc_client.cc
  asr.pb.cc
  asr.grpc.pb.cc
)
if (NOT MSVC)
  target_link_libraries(asr_grpc PUBLIC grpc++ grpc++_reflection utils)
else()
  target_link_libraries(asr_grpc PUBLIC gRPC::grpc++ gRPC::grpc++_reflection utils)
endif()

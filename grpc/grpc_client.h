#ifndef GRPC_GRPC_CLIENT_H_
#define GRPC_GRPC_CLIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include "grpc/asr.grpc.pb.h"
#include "utils/utils.h"

namespace mtasr {

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using asr::v1::ASR;
using asr::v1::Request;
using asr::v1::Response;

class GrpcClient {
 public:
  GrpcClient(const std::string& host, int port, const std::string& token="",
             int nbest=1, bool continuous_decoding=true, const std::string& req_id="test");
  void SendBinaryData(const void* data, size_t size);
  void ReadLoopFunc();
  void Join();
  bool done() const { return done_; }

 private:
  void Connect();
  std::string host_;
  int port_;
  std::string token_;
  std::shared_ptr<Channel> channel_{nullptr};
  std::unique_ptr<ASR::Stub> stub_{nullptr};
  std::shared_ptr<ClientContext> context_{nullptr};
  std::unique_ptr<ClientReaderWriter<Request, Response>> stream_{nullptr};
  std::shared_ptr<Request> request_{nullptr};
  std::shared_ptr<Response> response_{nullptr};

  std::string req_id_ = "";
  bool continuous_decoding_ = true;
  int nbest_ = 1;
  bool done_ = false;
  std::unique_ptr<std::thread> t_{nullptr};

  MTASR_DISALLOW_COPY_AND_ASSIGN(GrpcClient);
};

}  // namespace mtasr

#endif  // GRPC_GRPC_CLIENT_H_

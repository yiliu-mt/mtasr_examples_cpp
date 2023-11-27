#include "grpc/grpc_client.h"
#include "utils/log.h"

namespace mtasr {
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using asr::v1::Request;
using asr::v1::Response;

GrpcClient::GrpcClient(const std::string& host, int port, int nbest,
                       bool continuous_decoding, const std::string& req_id)
    : host_(host),
      port_(port),
      req_id_(req_id),
      nbest_(nbest),
      continuous_decoding_(continuous_decoding) {
  Connect();
  t_.reset(new std::thread(&GrpcClient::ReadLoopFunc, this));
}

void GrpcClient::Connect() {
  // connect to asr server
  channel_ = grpc::CreateChannel(host_ + ":" + std::to_string(port_),
                                 grpc::InsecureChannelCredentials());
  stub_ = ASR::NewStub(channel_);
  context_ = std::make_shared<ClientContext>();
  if (continuous_decoding_)
    stream_ = stub_->RealTimeSpeechRecognition(context_.get());
  else
    stream_ = stub_->OneSentenceSpeechRecognition(context_.get());

  // send config
  request_ = std::make_shared<Request>();
  response_ = std::make_shared<Response>();
  request_->mutable_decode_config()->set_req_id(req_id_);
  request_->mutable_decode_config()->set_nbest(nbest_);
  stream_->Write(*request_);
}

void GrpcClient::SendBinaryData(const void* data, size_t size) {
  //send binary data
  const int16_t* pdata = reinterpret_cast<const int16_t*>(data);
  request_->set_audio_data(pdata, size);
  stream_->Write(*request_);
}

void GrpcClient::ReadLoopFunc() {
  // listen thread
  try {
    while (stream_->Read(response_.get())) {
      // If something returned, we should parse the result
      for (int i = 0; i < response_->payload().result_size(); i++) {
        LOG(INFO) << i + 1 << "best " << response_->payload().result(i).sentence();

        // if SENTENCE_END is received, it means a sentence is ended.
        if (response_->header().type() == asr::v1::Response_Type_SENTENCE_END) {
          LOG(INFO) << i + 1 << "best sentence result: " << response_->payload().result(i).sentence();
          for (int j = 0; j < response_->payload().result(i).wordpieces_size(); j++) {
            LOG(INFO) << "word: " << response_->payload().result(i).wordpieces(j).word() << 
              " , start_time: " << response_->payload().result(i).wordpieces(j).start() <<
              " , end_time: " << response_->payload().result(i).wordpieces(j).end();
          }
        }
      }
      if (response_->header().status() != asr::v1::Response_Status_OK) {
        LOG(INFO) << "Req " << req_id_ << " : Error code " << response_->header().status();
        break;
      }
      // if COMPLETED is received, the recognition is finished.
      if (response_->header().type() == asr::v1::Response_Type_COMPLETED) {
        done_ = true;
        break;
      }
    }
  } catch (std::exception const& e) {
    LOG(ERROR) << e.what();
  }
}

void GrpcClient::Join() {
  stream_->WritesDone();
  t_->join();
  Status status = stream_->Finish();
  if (!status.ok()) {
    LOG(INFO) << "Recognize rpc failed.";
  }
}
}  // namespace mtasr

#ifndef WEBSOCKET_WEBSOCKET_CLIENT_H_
#define WEBSOCKET_WEBSOCKET_CLIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include <boost/asio/ssl/stream.hpp>
#include "boost/beast/core.hpp"
#include "boost/beast/websocket.hpp"
#include <boost/beast/websocket/ssl.hpp>

#include "utils/utils.h"

namespace mtasr {

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = boost::asio::ip::tcp;

class WsClient {
 public:
  WsClient(const std::string &mode, const std::string& url, const std::string& token);


  void set_configuration(
    const std::string& domain,
    const std::string& language,
    const std::string& format,
    const std::string& vocabulary_id,
    const std::string& lm_id,
    bool enable_punctuation,
    bool enable_itn,
    bool remove_disfluency,
    bool enable_speaker_info,
    int nbest,
    bool show_confidence,
    bool show_words,
    bool show_intermediate_result,
    bool enable_semantic_sentence_detection,
    const std::string& special_word_filter
  ) {
    domain_ = domain;
    language_ = language;
    format_ = format;
    vocabulary_id_ = vocabulary_id;
    lm_id_ = lm_id,
    enable_punctuation_ = enable_punctuation;
    enable_itn_ = enable_itn;
    remove_disfluency_ = remove_disfluency;
    enable_speaker_info_ = enable_speaker_info;
    nbest_ = nbest;
    show_confidence_ = show_confidence;
    show_words_ = show_words;
    show_intermediate_result_ = show_intermediate_result;
    enable_semantic_sentence_detection_ = enable_semantic_sentence_detection;
    special_word_filter_ = special_word_filter;
  }

  void SendTextData(const std::string& data);
  void SendBinaryData(const void* data, size_t size);
  void ReadLoopFunc();
  void Close();
  void Join();
  void SendStartSignal();
  void SendEndSignal();
  bool done() const { return done_; }

 private:
  void Connect();
  std::string scheme_;
  bool is_secure_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string token_;
  bool done_ = false;
  asio::io_context ioc_;
  // ws
  websocket::stream<tcp::socket> ws_{ioc_};
  // wss
  ssl::context ctx_{ssl::context::sslv23_client};
  websocket::stream<ssl::stream<tcp::socket>> wss_{ioc_, ctx_};
  std::unique_ptr<std::thread> t_{nullptr};


  // configuration
  std::string domain_ = "general";
  std::string language_ = "cn";
  std::string format_ = "pcm";
  std::string vocabulary_id_ = "";
  std::string lm_id_ = "";
  bool enable_punctuation_ = true;
  bool enable_itn_ = true;
  bool remove_disfluency_ = true;
  bool enable_speaker_info_ = false;
  int nbest_ = 1;
  bool show_confidence_ = false;
  bool show_words_ = true;
  bool show_intermediate_result_ = true;
  bool enable_semantic_sentence_detection_ = false;
  std::string special_word_filter_ = "";

  // overall result
  std::string final_result_ = "";

  MTASR_DISALLOW_COPY_AND_ASSIGN(WsClient);
};

}

#endif  // WEBSOCKET_WEBSOCKET_CLIENT_H_

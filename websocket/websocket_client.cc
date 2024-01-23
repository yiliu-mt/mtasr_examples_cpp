#include <regex>
#include "boost/json/src.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "websocket/websocket_client.h"
#include "utils/log.h"

namespace mtasr {

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;


void parse_url(const std::string& url, std::string& scheme, std::string& host, std::string& port, std::string& target)
{
    std::size_t scheme_end = url.find("://");
    if (scheme_end == std::string::npos)
        throw std::runtime_error("Invalid URL: No scheme found");
    scheme = url.substr(0, scheme_end);
    std::size_t host_start = scheme_end + 3; // Skip "://"
    std::size_t path_start = url.find("/", host_start);
    if (path_start == std::string::npos)
        path_start = url.size();
    host = url.substr(host_start, path_start - host_start);
    std::size_t port_start = host.find(":");
    if (port_start != std::string::npos) {
        port = host.substr(port_start + 1);
        host = host.substr(0, port_start);
    } else {
        port = (scheme == "wss") ? "443" : "80";
    }
    if (path_start < url.size())
        target = url.substr(path_start);
    else
        target = "/";
}

WsClient::WsClient(const std::string& url, const std::string& token)
    : token_(token) {
  parse_url(url, scheme_, host_, port_, path_);
  if (scheme_ != "ws" && scheme_ != "wss") {
    throw std::runtime_error("The websocket scheme must be ws or wss");
  }
  is_secure_ = (scheme_ == "wss");
  LOG(INFO) << "Scheme: " << scheme_ << ", host: " << host_ << ", path: " << path_ << ", port: " << port_;

  Connect();
  t_.reset(new std::thread(&WsClient::ReadLoopFunc, this));
}

void WsClient::Connect() {
  tcp::resolver resolver{ioc_};
  auto const results = resolver.resolve(host_, port_);

  if (is_secure_) {
    auto ep = asio::connect(wss_.next_layer().next_layer(), results);
    wss_.next_layer().handshake(ssl::stream_base::client);
    std::string host = host_ + ":" + std::to_string(ep.port());
    if (!token_.empty()) {
      std::string token = token_;
      wss_.set_option(websocket::stream_base::decorator(
        [token](websocket::request_type& req){
            req.set(http::field::authorization, token);
        })
      );
    }
    wss_.handshake(host_, path_);
  } else {
    auto ep = asio::connect(ws_.next_layer(), results);
    std::string host = host_ + ":" + std::to_string(ep.port());
    ws_.handshake(host_, path_);
  }
}

void WsClient::SendTextData(const std::string& data) {
  if (is_secure_) {
    wss_.text(true);
    wss_.write(asio::buffer(data));
  } else {
    ws_.text(true);
    ws_.write(asio::buffer(data));
  }
}

void WsClient::SendBinaryData(const void* data, size_t size) {
  if (is_secure_) {
    wss_.binary(true);
    wss_.write(asio::buffer(data, size));
  } else {
    ws_.binary(true);
    ws_.write(asio::buffer(data, size));
  }
}

void WsClient::Close() { 
  if (is_secure_) {
    wss_.close(websocket::close_code::normal);
  } else {
    ws_.close(websocket::close_code::normal);
  }
}

void WsClient::ReadLoopFunc() {
  try {
    while (true) {
      beast::flat_buffer buffer;
      if (is_secure_) {
        wss_.read(buffer);
      } else {
        ws_.read(buffer);
      }
      std::string message = beast::buffers_to_string(buffer.data());
      LOG(INFO) << message;
      if (is_secure_) {
        CHECK(wss_.got_text());
      } else {
        CHECK(ws_.got_text());
      }

      boost::property_tree::ptree json_obj;
      std::istringstream iss(message);
      boost::property_tree::read_json(iss, json_obj);

      int status = json_obj.get<int>("header.status", 0);
      std::string type = json_obj.get<std::string>("header.type");
      if (status != 1000) {
        LOG(ERROR) << "Error occurs. Check the status code: " << status << ". message: " << json_obj.get<std::string>("header.status_text");
        break;
      }
      if (type == "SentenceEnd") {
        std::string best_sentence;
        for (const auto& result_item : json_obj.get_child("payload.result")) {
          best_sentence = result_item.second.get<std::string>("text");
          break;
        }
        LOG(INFO) << "1-best sentence result: " << best_sentence;
        final_result_ += best_sentence;
      }
      if (type == "TranscriptionCompleted") {
        LOG(INFO) << "Final result\n" << final_result_;
        done_ = true;
        break;
      }
    }
  } catch (beast::system_error const& se) {
    // This indicates that the session was closed
    if (se.code() != websocket::error::closed) {
      LOG(ERROR) << se.code().message();
    }
  } catch (std::exception const& e) {
    LOG(ERROR) << e.what();
  }
}

void WsClient::Join() { t_->join(); }

void WsClient::SendStartSignal() {
  LOG(INFO) << nbest_;
  json::value start_tag = {
    {"header", {
        {"appid", token_},
        {"type", "StartTranscription"}
    }},
    {"payload", {
        {"domain", domain_},
        {"format", format_},
        {"language", language_},
        {"vocabulary_id", vocabulary_id_},
        {"lm_id", lm_id_},
        {"enable_punctuation", enable_punctuation_},
        {"enable_itn", enable_itn_},
        {"remove_disfluency", remove_disfluency_},
        {"enable_speaker_info", enable_speaker_info_},
        {"nbest", nbest_},
        {"show_words", show_words_},
        {"show_confidence", show_confidence_},
        {"show_intermediate_result", show_intermediate_result_},
        {"enable_semantic_sentence_detection", enable_semantic_sentence_detection_},
        {"special_word_filter", special_word_filter_}
    }}
  };
  std::string start_message = json::serialize(start_tag);
  this->SendTextData(start_message);
}

void WsClient::SendEndSignal() {
  json::value end_tag = {{"header", {{"appid", token_}, {"type", "StopTranscription"}}}};
  std::string end_message = json::serialize(end_tag);
  this->SendTextData(end_message);
}

}

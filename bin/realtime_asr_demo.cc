#include "frontend/wav.h"
#include "utils/flags.h"
#include "utils/string.h"
#include "websocket/websocket_client.h"

// Server
DEFINE_string(mode, "cloud", "the authorization mode");
DEFINE_string(url, "wss://api.mthreads.com/api/v1/asr", "url for the websocket server");
DEFINE_string(token, "", "token used for authorization");
DEFINE_string(wav_path, "", "test wav file path");
// Configuration
DEFINE_bool(enable_punctuation, true, "enable punctuation");
DEFINE_bool(enable_itn, true, "enable itn");
DEFINE_string(vocabulary_id, "", "vocabulary id");
DEFINE_bool(show_intermediate_result, true, "shwo intermediate result");
DEFINE_bool(show_words, true, "show words");
DEFINE_int32(nbest, 1, "nbest");
DEFINE_string(domain, "general", "domain");
DEFINE_string(language, "cn", "language");
DEFINE_string(format, "pcm", "audio format");
DEFINE_string(lm_id, "", "language model id");
DEFINE_bool(remove_disfluency, false, "remove disfluency");
DEFINE_bool(enable_speaker_info, false, "enable speaker information");
DEFINE_bool(show_confidence, false, "show confidence");
DEFINE_bool(enable_semantic_sentence_detection, false, "enable semantic sentence detection");
DEFINE_string(special_word_filter, "", "special word filter");
// End configuration
std::mutex g_mutex;

void decode(std::string &wav) {
  mtasr::WsClient client(
    FLAGS_mode,
    FLAGS_url,
    FLAGS_token
  );
  client.set_configuration(
    FLAGS_domain,
    FLAGS_language,
    FLAGS_format,
    FLAGS_vocabulary_id,
    FLAGS_lm_id,
    FLAGS_enable_punctuation,
    FLAGS_enable_itn,
    FLAGS_remove_disfluency,
    FLAGS_enable_speaker_info,
    FLAGS_nbest,
    FLAGS_show_confidence,
    FLAGS_show_words,
    FLAGS_show_intermediate_result,
    FLAGS_enable_semantic_sentence_detection,
    FLAGS_special_word_filter
  );

  // Send begin signal
  client.SendStartSignal();

  // Read wav
  mtasr::WavReader wav_reader(wav);
  const int num_samples = wav_reader.num_samples();
  const int sample_rate = 16000;
  // Only support 16K
  CHECK_EQ(wav_reader.sample_rate(), sample_rate);
  std::vector<float> pcm_data(wav_reader.data(),
                              wav_reader.data() + num_samples);

  // Send data every 0.16 second
  const float interval = 0.16;
  const int sample_interval = interval * sample_rate;
  for (int start = 0; start < num_samples; start += sample_interval) {
    if (client.done()) {
      break;
    }
    int end = std::min(start + sample_interval, num_samples);
    // Convert to short
    std::vector<int16_t> data;
    data.reserve(end - start);
    for (int j = start; j < end; j++) {
      data.push_back(static_cast<int16_t>(pcm_data[j]));
    }

    // Send PCM data
    client.SendBinaryData(data.data(), data.size() * sizeof(int16_t));
    LOG(INFO) << "Send " << data.size() << " samples";
    
    // If you want to simulate the practical application,
    // uncomment the following lines.
    // This will make the client to wait before sending the next sample
    // std::this_thread::sleep_for(
    //     std::chrono::milliseconds(static_cast<int>(interval * 1000)));
  }

  // Send end signal
  client.SendEndSignal();
  client.Join();
  return;
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);
  google::InitGoogleLogging(argv[0]);
  decode(FLAGS_wav_path);
}


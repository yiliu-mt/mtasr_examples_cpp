#include "frontend/wav.h"
#include "utils/flags.h"
#include "utils/string.h"
#include "grpc/grpc_client.h"

DEFINE_string(hostname, "127.0.0.1", "hostname of websocket server");
DEFINE_int32(port, 10086, "port of websocket server");
DEFINE_string(wav_path, "", "test wav file path");

void decode(std::string &wav) {
  mtasr::GrpcClient client(
    FLAGS_hostname,
    FLAGS_port
  );

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
  client.Join();
  return;
}


int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);
  google::InitGoogleLogging(argv[0]);
  decode(FLAGS_wav_path);
}

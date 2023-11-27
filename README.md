# MT ASR C++ client

## Compile

``` sh
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
```

## Run

Input wav only support 16kHz wav format files.

You should also specify the URL and port of the ASR server.

``` sh
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
    --hostname URL \
    --port PORT \
    --wav_path $your_test_wav_path 2>&1
```


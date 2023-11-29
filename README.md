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

## Docker

It is more convenient to use a docker image: `yiliumt/mtasr_client:v1.0`

This image can be used directly:

```
docker run --rm -it yiliumt/mtasr_client:v1.0 bash

# run
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
    --hostname URL \
    --port PORT \
    --wav_path $your_test_wav_path 2>&1
```

To build from scratch, you can try this in the image:

```
rm -r build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
```

Note, it may also try to `git clone` grpc library from github. You need to change cmake/grpc.cmake to your local file.

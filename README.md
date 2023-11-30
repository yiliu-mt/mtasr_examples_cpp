# MT ASR C++ client

## Compile

``` sh
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
```

## Run

Input wav only support 16kHz wav format files.

You should also specify the URL and port of the ASR server.

If a authorization token is given, you should specify the TOKEN as well.

``` sh
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
    --hostname URL \
    --port PORT \
    --token TOKEN \
    --wav_path $your_test_wav_path 2>&1
```

## Docker

It is more convenient to use a docker image: `yiliumt/mtasr_client:v1.1`

This image can be used directly:

``` sh
docker run --rm -it yiliumt/mtasr_client:v1.1 bash

# run
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
    --hostname URL \
    --port PORT \
    --token TOKEN \
    --wav_path $your_test_wav_path 2>&1
```

To build from scratch, you can try this in the image:

``` sh
rm -r build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
```

> Note, it may also try to `git clone` grpc library from github. 

### How to use the libraries in the image

The third-party libraries are downloaded in `/workspace/mtasr_examples_cpp/fc_base`. You can re-use these files by copying this folder to your own local directory.

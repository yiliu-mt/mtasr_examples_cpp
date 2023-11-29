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

Note, it may also try to `git clone` grpc library from github. To disable the downloading, please change `cmake/grpc.cmake` to:

```
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/grpc)
# third_party: grpc
# On how to build grpc, you may refer to https://github.com/grpc/grpc
# We recommend manually recursive clone the repo to avoid internet connection problem
# FetchContent_Declare(gRPC
#   GIT_REPOSITORY https://github.com/grpc/grpc
#   GIT_TAG        v1.37.1
# )
# FetchContent_MakeAvailable(gRPC)
```

### How to use the libraries in the image

The third-party libraries are downloaded in `/workspace/mtasr_examples_cpp/fc_base`. You can copy this folder to your own local directory to avoid downloading during building. You should also need to modify `cmake/grpc.cmake` to make it work.

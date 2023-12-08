# MT ASR C++ client

> NOTE: This branch is not compatible with the main branch. Use this branch independently.

This branch is created for building the binary under:

1. CentOS 7

2. cmake 2.8.12

3. gcc 4.8.5

You also have to install some basic libraries to make the binary. In centos 7, you can install using yum:

```
yum install make automake gcc gcc-c++ kernel-devel which libtool
```


## Build dependencies

This binary depends on three libraries. Build them independently. Check `lib/README.md` to see the download links.

### gflags

``` sh
tar zxvf gflags-2.2.2-cmake-2.8.tar.gz
cd gflags-2.2.2-cmake-2.8
mkdir build &&  cd build/  &&  cmake .. && cmake --build .
```

### glog

``` sh
tar zxvf glog-0.4.0-cmake-2.8.tar.gz
cd glog-0.4.0-cmake-2.8
mkdir build &&  cd build/  &&  cmake .. && cmake --build .
```

### gRPC

``` sh
tar zxvf grpc-1.12.1.tar.gz
cd grpc
make
```

## Build ASR client

``` sh
cd mtasr_examples_cpp
mkdir build &&  cd build/  &&  cmake .. && cmake --build .
```

## Run

> If you don't have the token, do not need to specify `--token TOKEN` in the command line.

``` sh
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
    --hostname URL \
    --port PORT \
    --token TOKEN \
    --wav_path $your_test_wav_path 2>&1
```


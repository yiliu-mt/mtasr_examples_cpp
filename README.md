# MT ASR C++ client

> NOTE: This branch is not compatible with the main branch. Use this branch independently.

This branch is created for building the binary in the following environment:

- CentOS 7
- CMake 2.8.12
- gcc 4.8.5

You also have to install some basic libraries to make the binary. In centos 7, you can install using yum:

```
yum install wget make automake gcc gcc-c++ kernel-devel which libtool cmake openssl-devel
```


## Build dependencies

This binary depends on third-party libraries. They should be built independently. Please follow the instructions.

### gflags

``` sh
cd ${this_project_dir}
mkdir -p lib && cd lib
wget https://mt-ai-speech-public.tos-cn-beijing.volces.com/gflags-2.2.2-cmake-2.8.tar.gz
tar zxvf gflags-2.2.2-cmake-2.8.tar.gz
cd gflags-2.2.2-cmake-2.8
mkdir build &&  cd build/  &&  cmake .. && cmake --build .
```

### glog

``` sh
cd ${this_project_dir}
mkdir -p lib && cd lib
wget https://mt-ai-speech-public.tos-cn-beijing.volces.com/glog-0.4.0-cmake-2.8.tar.gz
tar zxvf glog-0.4.0-cmake-2.8.tar.gz
cd glog-0.4.0-cmake-2.8
mkdir build &&  cd build/  &&  cmake .. && cmake --build .
```

### boost

``` sh
cd ${this_project_dir}
mkdir -p lib && cd lib
wget https://mt-ai-speech-public.tos-cn-beijing.volces.com/boost_1_75_0.tar.gz
tar zxvf boost_1_75_0.tar.gz
```

## Compile

``` sh
cd ${this_project_dir}
mkdir build &&  cd build/ && cmake .. && cmake --build .
```

## Run

Refer to [here](https://github.com/yiliu-mt/mtasr_examples_cpp/tree/main?tab=readme-ov-file#real-time-streaming-asr) for the detailed usage of this demo.


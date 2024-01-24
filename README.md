# MT ASR C++ client

## Linux

### Requirements

1. CMake >= 3.14

2. gcc >= 9; <11

3. glibc >= 2.31

4. OpenSSL >= 3

For example, in Ubuntu 20.04, you can install the dependencies using `apt install cmake build-essential libssl-dev`.

### Compile

Simply run

``` sh
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
```

#### Dependencies

To build this client, you need the following libraries: `gflags`, `glog` and `boost`. The dependencies will be automatically downloaded during cmake build. Other dependencies have been installed as the system libraries.

If you would like to build this client in a offline environment, please check `cmake/*.cmake` to find the download link. After all the files have been downloaded manually, edit the `.cmake` to refer to the local files. For instance, if `gflags-2.2.2.zip` are downloaded and placed in 

``` sh
/home/user/gflags-2.2.2.zip
```

Then edit `cmake/gflags.cmake` as

``` cmake
# before 
# URL  https://mt-ai-speech-public.tos-cn-beijing.volces.com/gflags-2.2.2.zip
# after:
URL  /home/user/gflags-2.2.2.zip
```


### Real-time streaming ASR

#### Run demo

The input only supports 16kHz 16bit mono wav file.

You should specify the URL and the token for authorization. The real-time streaming API supports different configurations. Please refer to [bin/realtime_asr_demo.cc](bin/realtime_asr_demo.cc) for details. Also, you can use

``` sh
./build/bin/realtime_asr_demo --help
```

to see all the parameters. To run the demo, use

``` sh
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
  --url wss://api.mthreads.com/api/v1/asr \
  --token $your_token \
  --wav_path demo.wav \
  --enable_punctuation true \
  --enable_itn true
```

#### Docker

It is more convenient to use a docker image: `yiliumt/mtasr_client:v1.2`

This image can be used directly:

``` sh
docker run --rm -it yiliumt/mtasr_client:v1.2 bash

# run
export GLOG_logtostderr=1
export GLOG_v=2
./build/bin/realtime_asr_demo \
  --url wss://api.mthreads.com/api/v1/asr \
  --token $your_token \
  --wav_path demo.wav \
  --enable_punctuation true \
  --enable_itn true
```

## Windows

In Windows, we strongly recommand to use vcpkg. [Vcpkg](https://github.com/microsoft/vcpkg) is a tool that you can install and manage different c++ libraries. With vcpkg, the dependencies can be installed much easier. We will describe how to build this client in Windows, using vcpkg.

### Requirements

1. CMake >= 3.14

2. Visual Studio >= 2019

3. OpenSSL >= 3

#### Install dependencies using vcpkg

1. Install CMake and Visual Studio

2. Install vcpkg 

    Following the [instructions](https://github.com/microsoft/vcpkg#quick-start-windows). After installing, you can add the path of vcpkg to your PATH environment variable.

3. Install OpenSSL

    Run the following commands in PowerShell (Assuming you are using Windows x64):

    ```
    vcpkg install openssl:x64-windows
    vcpkg integrate install
    ```

    After installing, you can build projects with grpc by adding `-DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_path>\scripts/buildsystems/vcpkg.cmake` when build with cmake. We will describe the details later.

### Compile

#### Generate configuration using CMake GUI

1. Open cmake, add this directory to "Where is the source code".

2. add <this_project_path>/build to "Where to build the binaries".

    ![overview](misc/windows/cmake_overview.png)

3. Click "Add Entry":

    - `Name`: "CMAKE_TOOLCHAIN_FILE"
    - `Type`: "FILEPATH"
    - `Value`: <vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake

    ![add_entry](misc/windows/cmake_add_entry.png)

    - `Name`: "MSVC"
    - `Type`: "BOOL"
    - `Value`: checked

    ![add_entry](misc/windows/cmake_add_msvc.png)

    - `Name`: "OPENSSL_INCLUDE_DIR"
    - `Type`: "PATH"
    - `Value`: <vcpkg_install_path>/installed/x64-windows/include

    ![add_entry](misc/windows/cmake_add_openssl_include_dir.png)

    - `Name`: "OPENSSL_LIBRARIES"
    - `Type`: "PATH"
    - `Value`: <vcpkg_install_path>/installed/x64-windows/lib

    ![add_entry](misc/windows/cmake_add_openssl_libraries.png)


4. Click "Configure". Choose the compiler version depending on your demand. 

    ![choose_compiler](misc/windows/cmake_choose_compiler.png)

5. Click "Generate". Once finished, click "Open Project".

    ![click_generate](misc/windows/cmake_generate.png)

#### Build using Visual Studio

In VS, "Build" -> "Build Solution (F7)". It will build the project using the configuration genereated by cmake. 

![vs_build](misc/windows/vs_build.png)

After building, the executable file will be generated in `<this_project_path>/buid/bin/<Debug or Release>/realtime_asr_demo.exe`.

### Run

Use powershell to run the demo. Please refer to the Linux section for the usage.

For example, to run the real-time streaming ASR demo:

``` powershell
$env:GLOG_logtostderr=1
$env:GLOG_v=2
./build/bin/<Debug or Release>/realtime_asr_demo.exe \
  --url wss://api.mthreads.com/api/v1/asr \
  --token $your_token \
  --wav_path demo.wav \
  --enable_punctuation true \
  --enable_itn true
```

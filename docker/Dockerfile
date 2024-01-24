FROM ubuntu:20.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y git cmake wget build-essential libssl-dev
RUN git clone https://github.com/yiliu-mt/mtasr_examples_cpp.git /workspace/mtasr_examples_cpp
WORKDIR /workspace/mtasr_examples_cpp
RUN mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
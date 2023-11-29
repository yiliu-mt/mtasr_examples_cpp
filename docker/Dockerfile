FROM ubuntu:20.04
RUN apt-get update && apt-get install -y git cmake wget build-essential
RUN git clone https://github.com/yiliu-mt/mtasr_examples_cpp.git /workspace/mtasr_examples_cpp
WORKDIR /workspace/mtasr_examples_cpp
RUN mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release
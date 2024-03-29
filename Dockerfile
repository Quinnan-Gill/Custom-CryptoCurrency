FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get -y install \
    gcc g++ cmake make \
    git m4 wget lzip \
    autoconf automake libtool curl \
    unzip


# Install Cypto++
RUN git clone https://github.com/weidai11/cryptopp.git
RUN cd cryptopp/ && make && make test && make install

# Install GMP (big integer)
RUN git clone https://github.com/alisw/GMP.git
RUN cd GMP/ && ./configure --enable-cxx && make && make check && make install

RUN git clone --recurse-submodules https://github.com/google/leveldb.git
RUN mkdir -p leveldb/build
RUN cd leveldb/build/ && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . && \
    make install

RUN git clone https://github.com/protocolbuffers/protobuf.git
RUN cd protobuf/ && \
    git submodule update --init --recursive && \
    ./autogen.sh && \
    ./configure && make && \
    make install && ldconfig

RUN apt-get install libssl-dev

WORKDIR /home
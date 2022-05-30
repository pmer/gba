FROM devkitpro/devkitarm
WORKDIR /build
COPY CMakeLists.txt /build/
COPY src /build/src
RUN \
    mkdir build && \
    cd build && \
    cmake .. && \
    make VERBOSE=1

FROM devkitpro/devkitarm
WORKDIR /build
COPY CMakeLists.txt /build/
COPY src /build/src
RUN \
    mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make VERBOSE=1

FROM devkitpro/devkitarm
RUN \
    export DEVKITPRO=/opt/devkitpro && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make VERBOSE=1 && \
    echo "Built $PWD/untitled.gba"

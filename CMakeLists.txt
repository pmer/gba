cmake_minimum_required(VERSION 3.18)
include($ENV{DEVKITPRO}/cmake/devkitARM.cmake)
project(playground)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        playground
        src/gfx.cpp
        src/gfx.h
        src/hw/audio.h
        src/hw/int.h
        src/hw/mm.h
        src/hw/video.h
        src/main.cpp
        src/math.cpp
        src/math.h
        src/mgba.cpp
        src/pong.cpp
        src/pong.h
        src/print.h
        src/song.cpp
        src/song.h
        src/tonc.h
)

#add_custom_target(run COMMAND $ENV{MGBA} ${CMAKE_CURRENT_BINARY_DIR}/playground.gba)
#add_custom_target(debug COMMAND $ENV{MGBA} -g ${CMAKE_CURRENT_BINARY_DIR}/playground.gba)

set(CMAKE_EXECUTABLE_SUFFIX .elf)

add_custom_command(
        TARGET playground
        POST_BUILD
        COMMAND ${DEVKITPRO}/devkitARM/bin/arm-none-eabi-objcopy -O binary playground.elf playground.gba
)

add_custom_command(
        TARGET playground
        POST_BUILD
        COMMAND ${DEVKITPRO}/tools/bin/gbafix playground.gba
)

set(COMPILE_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork -ffast-math")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMPILE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMPILE_FLAGS} -fno-rtti -fno-exceptions")

set(FLAGS_RELEASE "-fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -fvisibility=hidden -fno-stack-protector -flto")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} ${FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS_RELEASE}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=gba.specs")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_RELEASE_FLAGS} -flto")

#target_link_directories(playground PUBLIC ${DEVKITPRO}/libgba/lib)
#target_link_libraries(playground mm)
#target_link_libraries(playground gba)

target_link_directories(playground PUBLIC ${DEVKITPRO}/libtonc/lib)
target_include_directories(playground PUBLIC ${DEVKITPRO}/libtonc/include)
target_link_libraries(playground tonc)

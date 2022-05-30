cmake_minimum_required(VERSION 3.18)
include($ENV{DEVKITPRO}/cmake/devkitARM.cmake)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        untitled
        src/main.cpp
        src/mgba.cpp
        src/print.h
)

#add_custom_target(run COMMAND $ENV{MGBA} ${CMAKE_CURRENT_BINARY_DIR}/untitled.gba)
#add_custom_target(debug COMMAND $ENV{MGBA} -g ${CMAKE_CURRENT_BINARY_DIR}/untitled.gba)

#set(CMAKE_LINK_FLAGS "-specs=gba.specs -L${DEVKITPRO}/libgba/lib -lmm -lgba")

set(CMAKE_EXECUTABLE_SUFFIX .elf)

#set(CMAKE_ASM_OUTPUT_EXTENSION .o)
#set(CMAKE_C_OUTPUT_EXTENSION .o)
#set(CMAKE_CXX_OUTPUT_EXTENSION .o)

#set(CMAKE_ASM_OUTPUT_EXTENSION_REPLACE 1)
#set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
#set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

#set(CMAKE_CXX_LINK_EXECUTABLE
#        "<CMAKE_C_COMPILER> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
#set(CMAKE_CXX_LINK_EXECUTABLE
#        "${CMAKE_CXX_LINK_EXECUTABLE}; ${DEVKITPRO}/devkitARM/bin/arm-none-eabi-objcopy -O binary main.elf main.gba")
#set(CMAKE_CXX_LINK_EXECUTABLE
#        "${CMAKE_CXX_LINK_EXECUTABLE}; ${DEVKITPRO}/tools/bin/gbafix main.gba")

add_custom_command(
        TARGET untitled
        POST_BUILD
        COMMAND ${DEVKITPRO}/devkitARM/bin/arm-none-eabi-objcopy -O binary untitled.elf untitled.gba
)

add_custom_command(
        TARGET untitled
        POST_BUILD
        COMMAND ${DEVKITPRO}/tools/bin/gbafix untitled.gba
)

# From 3rd-party Makefile:
#arm-none-eabi-gcc -MMD -MP -MF /c/GBA_VSCode_Basic/build/main.d -g -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -mthumb -mthumb-interwork -iquote /c/GBA_VSCode_Basic/include -iquote /c/GBA_VSCode_Basic/resources -I/opt/devkitpro/libgba/include -I/c/GBA_VSCode_Basic/build -c /c/GBA_VSCode_Basic/source/main.c -o main.o
#arm-none-eabi-gcc  -g -mthumb -mthumb-interwork -Wl,-Map,GBA_VSCode_Basic.map -specs=gba.specs   main.o  -L/opt/devkitpro/libgba/lib -lmm -lgba -o /c/GBA_VSCode_Basic/GBA_VSCode_Basic.elf

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=gba.specs")

#target_link_directories(untitled PUBLIC ${DEVKITPRO}/libgba/lib)
#target_link_libraries(untitled mm)
#target_link_libraries(untitled gba)

target_link_directories(untitled PUBLIC ${DEVKITPRO}/libtonc/lib)
target_include_directories(untitled PUBLIC ${DEVKITPRO}/libtonc/include)
target_link_libraries(untitled tonc)


# Core components:
#\devkitPro\devkitARM\bin\arm-none-eabi-gcc
#    -mcpu=arm7tdmi
#    -mtune=arm7tdmi
#    -c source/main.c
#    -o build/main.o
#\devkitPro\devkitARM\bin\arm-none-eabi-gcc
#    -Wl,-Map,build/GBA_VSCode_Basic.map
#    -specs=gba.specs
#    build/main.o
#    -LC:/devkitpro/libgba/lib
#    -lmm
#    -lgba
#    -o GBA_VSCode_Basic.elf
#\devkitPro\devkitARM\bin\arm-none-eabi-objcopy
#    -O binary
#    GBA_VSCode_Basic.elf
#    GBA_VSCode_Basic.gba
#\devkitPro\tools\bin\gbafix
#    GBA_VSCode_Basic.elf

#"C:\Users\Paul Merrill\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\221.5591.52\bin\cmake\win\bin\cmake.exe" --build . --verbose --clean-first
#"C:\Users\Paul Merrill\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\221.5591.52\bin\cmake\win\bin\cmake.exe" --build . --verbose --target debug

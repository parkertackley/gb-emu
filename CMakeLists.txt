cmake_minimum_required(VERSION 3.23)
project(gb-emu LANGUAGES C)

# vcpkg automatically hooks into these calls!
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SOURCES
        src/bus.c
        src/cart.c
        src/cpu.c
        src/emu.c
        src/instructions.c
        src/main.c
        src/ppu.c
        src/timer.c
        src/cpu_util.c
        src/cpu_proc.c
        src/cpu_fetch.c
        src/ram.c
        includes/ram.h
)

add_executable(gb-emu ${SOURCES})

target_include_directories(gb-emu PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/includes"
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(gb-emu PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)
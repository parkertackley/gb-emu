cmake_minimum_required(VERSION 3.23)

project(gb-emu LANGUAGES C)

# Find SDL2 headers (still useful for compiler flags)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Grab all C source files
file(GLOB SOURCES "src/*.c")

# Create the executable
add_executable(gb-emu ${SOURCES})

# Include directories (your own headers + SDL2)
target_include_directories(gb-emu PRIVATE includes ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Link SDL2 libraries explicitly
target_link_libraries(gb-emu PRIVATE SDL2 SDL2_ttf)

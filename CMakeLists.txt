cmake_minimum_required(VERSION 3.20)

project(bkfetch
    VERSION 0.1.0
    DESCRIPTION "BoykisserFetch"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "main.cpp" "src/*.cpp")

add_executable(bkfetch ${SOURCES})

target_include_directories(bkfetch PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
)

target_compile_options(bkfetch PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

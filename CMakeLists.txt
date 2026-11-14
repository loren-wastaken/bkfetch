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
enable_testing()

# 1. Verify every vendor/device in pciDatabase resolves
add_test(
    NAME TestAllGPUModels
    COMMAND bkfetch --test-all-gpumodels
)

# 2. Verify known vendor + unknown device falls back cleanly
add_test(
    NAME TestUnknownDevice
    COMMAND bkfetch --test-gpumodels 0x10DE 0x9999
)
set_tests_properties(TestUnknownDevice PROPERTIES
    PASS_REGULAR_EXPRESSION "Vendor: 0x10DE \\(NVIDIA\\).*Device: 0x9999 \\(Unknown device\\)"
)

# 3. Verify unknown vendor + unknown device falls back cleanly
add_test(
    NAME TestUnknownVendor
    COMMAND bkfetch --test-gpumodels 0xFFFF 0xFFFF
)
set_tests_properties(TestUnknownVendor PROPERTIES
    PASS_REGULAR_EXPRESSION "Vendor: 0xFFFF \\(Unknown vendor\\).*Device: 0xFFFF \\(Unknown device\\)"
)

# 4. Verify invalid hex input exits with error code
add_test(
    NAME TestInvalidHex
    COMMAND bkfetch --test-gpumodels ZZZZ 0x28E0
)
set_tests_properties(TestInvalidHex PROPERTIES WILL_FAIL TRUE)

# 5. Verify missing arguments exits with error code
add_test(
    NAME TestMissingArgs
    COMMAND bkfetch --test-gpumodels 0x10DE
)
set_tests_properties(TestMissingArgs PROPERTIES WILL_FAIL TRUE)

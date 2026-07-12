cmake_minimum_required(VERSION 3.21)

project(
    archlab_rvsoc_sim
    VERSION 0.1.0
    DESCRIPTION "A modular research-oriented RISC-V SoC simulator"
    LANGUAGES CXX
)

option(ARCHLAB_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(ARCHLAB_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectOptions)

add_library(archlab_sim STATIC)
add_library(archlab::sim ALIAS archlab_sim)

target_sources(
    archlab_sim
    PRIVATE
        src/cpu/simple_core.cpp
        src/devices/magic_device.cpp
        src/devices/simple_ram.cpp
        src/kernel/event_queue.cpp
        src/platform/address_space.cpp
        src/stats/stats_manager.cpp
)

target_include_directories(
    archlab_sim
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(archlab_sim PUBLIC cxx_std_20)
archlab_set_project_warnings(archlab_sim)
archlab_enable_sanitizers(archlab_sim)

add_subdirectory(app)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

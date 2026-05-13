cmake_minimum_required(VERSION 3.16)

project(accel-stream)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED COMPONENTS system)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(spdlog)

add_subdirectory(core)
add_subdirectory(clientA)
add_subdirectory(clientB)
add_subdirectory(server)


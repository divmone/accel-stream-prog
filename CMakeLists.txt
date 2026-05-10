cmake_minimum_required(VERSION 3.16)

project(accel-stream)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG master
)

FetchContent_MakeAvailable(asio)

add_subdirectory(core)
add_subdirectory(clientA)
add_subdirectory(clientB)
add_subdirectory(server)



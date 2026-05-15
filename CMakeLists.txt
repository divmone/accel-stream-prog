cmake_minimum_required(VERSION 3.28)

project(accel-stream)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_FETCH_BOOST "Fetch Boost via FetchContent instead of find_package" OFF)

include(FetchContent)

if(USE_FETCH_BOOST)
    message(STATUS "Boost: using FetchContent (this will download Boost on first configure)")
    set(BOOST_INCLUDE_LIBRARIES system asio)
    FetchContent_Declare(
            Boost
            URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    )
    FetchContent_MakeAvailable(Boost)
else()
    message(STATUS "Boost: using find_package (set -DUSE_FETCH_BOOST=ON to fetch instead)")
    find_package(Boost REQUIRED COMPONENTS system)
    # Asio is header-only; older FindBoost / system installs may not define Boost::asio.
    # Provide a compatibility alias so downstream code can always link Boost::asio.
    if(NOT TARGET Boost::asio)
        add_library(Boost::asio INTERFACE IMPORTED)
        target_link_libraries(Boost::asio INTERFACE Boost::headers)
    endif()
endif()

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.17.0
)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

FetchContent_MakeAvailable(spdlog json)

add_subdirectory(core)
add_subdirectory(clientA)
add_subdirectory(clientB)
add_subdirectory(server)


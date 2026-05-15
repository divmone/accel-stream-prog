project(core)

add_library(core STATIC
        Protocol.cpp
        TcpConnection.cpp
        TcpConnection.hpp
        CoroRuntime.hpp
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC Boost::system Boost::asio spdlog::spdlog nlohmann_json::nlohmann_json)
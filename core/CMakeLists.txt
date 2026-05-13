project(core)

add_library(core STATIC
        Protocol.cpp
        TcpConnection.cpp
        TcpConnection.hpp
        CoroRuntime.hpp
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/third-party
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(core PUBLIC ${Boost_LIBRARIES} spdlog::spdlog)
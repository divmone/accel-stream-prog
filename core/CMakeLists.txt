project(core)

add_library(core STATIC
    Packet.cpp
    TcpClient.cpp
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/third-party
)
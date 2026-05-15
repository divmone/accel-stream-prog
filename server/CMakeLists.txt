project(server)
add_executable(${PROJECT_NAME}  src/main.cpp
        src/AsyncServer.cpp
        src/AsyncServer.hpp
        src/Session.cpp
        src/Session.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
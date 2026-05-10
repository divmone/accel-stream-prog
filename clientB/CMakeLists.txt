project(clientB)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/AccelService.cpp
        src/AccelService.hpp)

target_link_libraries(clientB PRIVATE core)

project(clientA)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/AccelService.hpp
        src/ModuleLogger.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE core)

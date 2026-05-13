project(clientA)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/AccelService.cpp
        src/AccelService.hpp)

target_include_directories(${PROJECT_NAME}  PRIVATE
        ${CMAKE_SOURCE_DIR}/third-party
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)

project(clientA)
add_executable(${PROJECT_NAME}  src/main.cpp)

target_include_directories(${PROJECT_NAME}  PRIVATE
        ${CMAKE_SOURCE_DIR}/third-party
)

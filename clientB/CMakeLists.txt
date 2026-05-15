project(clientB)
add_executable(${PROJECT_NAME}  src/main.cpp
        src/ModuleService.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE core)

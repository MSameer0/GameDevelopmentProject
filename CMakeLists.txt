cmake_minimum_required(VERSION 3.10)

project(GameDevelopmentProject)

set(PROJECT_SOURCE_FILES
    source/main.cpp
)

#add the application
include_directories(source)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

#add engine subdirectory
add_subdirectory(engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")

#link to engine
target_link_libraries(${PROJECT_NAME}
    Engine
)
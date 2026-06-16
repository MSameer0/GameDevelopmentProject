cmake_minimum_required(VERSION 3.10)

project(GameDevelopmentProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GLFW library
add_subdirectory(thirdparty/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")

# Add GLEW library
add_compile_definitions(GLEW_NO_GLU)
set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(thirdparty/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")

# Main program
add_executable(MainGame
        source/main.cpp
)

target_include_directories(MainGame PRIVATE
        source
        thirdparty/glfw-3.4/include
        thirdparty/glew/include
)

target_link_libraries(MainGame
        glfw
        glew_s
)

# Assignment program
add_executable(Assignment
        source/Assignment.cpp
)

target_include_directories(Assignment PRIVATE
        source
        thirdparty/glfw-3.4/include
        thirdparty/glew/include
)

target_link_libraries(Assignment
        glfw
        glew_s
)
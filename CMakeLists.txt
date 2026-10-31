cmake_minimum_required(VERSION 3.20)
project(OpenGLProject)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to look for header files (.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gather all source files in the src folder
file(GLOB SOURCES 
    "src/*.cpp"
    "src/*.c"
)

# Create the executable program
add_executable(OpenGLProject ${SOURCES})

# Link GLFW, OpenGL, and Windows system libraries required by MSVC
if(WIN32)
    target_link_libraries(OpenGLProject PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
        opengl32.lib
        gdi32.lib
        shell32.lib
        user32.lib
    )
endif()
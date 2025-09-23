cmake_minimum_required(VERSION 3.16)
project(MyUILibrary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Find SFML (installed via vcpkg)
find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

# Build static library from src/
add_library(MyUI STATIC
    src/Element.cpp

    # add more source files here
)

# Expose public headers
target_include_directories(MyUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against SFML
target_link_libraries(MyUI
    PUBLIC SFML::Graphics SFML::Window SFML::System
)

# Example app
add_executable(SimpleButton examples/main.cpp)
target_link_libraries(SimpleButton PRIVATE MyUI)

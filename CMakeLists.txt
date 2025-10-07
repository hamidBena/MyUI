cmake_minimum_required(VERSION 3.16)
project(MyUILibrary LANGUAGES CXX VERSION 1.0.0)

# Set project version
set(MYUI_VERSION_MAJOR 1)
set(MYUI_VERSION_MINOR 0)
set(MYUI_VERSION_PATCH 0)
set(MYUI_VERSION ${MYUI_VERSION_MAJOR}.${MYUI_VERSION_MINOR}.${MYUI_VERSION_PATCH})

# Options for library type
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(MYUI_BUILD_EXAMPLES "Build example applications" ON)
option(MYUI_INSTALL "Generate installation target" ON)

# Find SFML (installed via vcpkg)
find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

# Automatically discover all source files
file(GLOB_RECURSE MYUI_SOURCES "src/*.cpp")
file(GLOB_RECURSE MYUI_HEADERS "include/*.hpp")

message(STATUS "MyUI Library Version: ${MYUI_VERSION}")
message(STATUS "Sources found: ${MYUI_SOURCES}")
message(STATUS "Headers found: ${MYUI_HEADERS}")

# Build library
add_library(MyUI ${MYUI_SOURCES})

# Set library properties
set_target_properties(MyUI PROPERTIES
    VERSION ${MYUI_VERSION}
    SOVERSION ${MYUI_VERSION_MAJOR}
    OUTPUT_NAME "MyUI"
)

# Include directories
target_include_directories(MyUI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link against SFML
target_link_libraries(MyUI
    PUBLIC SFML::Graphics SFML::Window SFML::System
)

# Compile features
target_compile_features(MyUI PUBLIC cxx_std_23)

# Installation targets
if(MYUI_INSTALL)
    include(GNUInstallDirs)
    
    # Install library
    install(TARGETS MyUI
        EXPORT MyUITargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    
    # Install headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )
    
    # Install CMake config files
    install(EXPORT MyUITargets
        FILE MyUITargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )
    
    # Create config file for find_package
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MyUIConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/MyUIConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )
    
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/MyUIConfigVersion.cmake
        VERSION ${MYUI_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/MyUIConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/MyUIConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )
endif()

# Example application
if(MYUI_BUILD_EXAMPLES)
    add_executable(SimpleButton examples/main.cpp)
    target_link_libraries(SimpleButton PRIVATE MyUI)
    
    # Install example if requested
    if(MYUI_INSTALL)
        install(TARGETS SimpleButton
            DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    endif()
endif()

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

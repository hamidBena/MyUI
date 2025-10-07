cmake_minimum_required(VERSION 3.16)
project(MyUILibrary LANGUAGES CXX VERSION 1.0.0)

# Enable FetchContent for newer CMake versions
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16")
    cmake_policy(SET CMP0135 NEW)
endif()

set(MYUI_VERSION_MAJOR 1)
set(MYUI_VERSION_MINOR 0)
set(MYUI_VERSION_PATCH 0)
set(MYUI_VERSION ${MYUI_VERSION_MAJOR}.${MYUI_VERSION_MINOR}.${MYUI_VERSION_PATCH})

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(MYUI_BUILD_EXAMPLES "Build example applications" ON)
option(MYUI_INSTALL "Generate installation target" ON)

# ---- Dependencies ----
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.x)
FetchContent_MakeAvailable(SFML)

# ---- Source Files ----
file(GLOB_RECURSE MYUI_SOURCES "src/*.cpp")
file(GLOB_RECURSE MYUI_HEADERS "include/*.hpp")

# ---- Library ----
add_library(MyUI ${MYUI_SOURCES})

set_target_properties(MyUI PROPERTIES
    VERSION ${MYUI_VERSION}
    SOVERSION ${MYUI_VERSION_MAJOR}
    OUTPUT_NAME "MyUI"
)

target_include_directories(MyUI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(MyUI
    PUBLIC SFML::Graphics SFML::Window SFML::System
)

target_compile_features(MyUI PUBLIC cxx_std_23)

# ---- Install / Export / Config ----
if(MYUI_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install the library and export include paths
    install(TARGETS MyUI
        EXPORT MyUITargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}  # ✅ FIX: include path properly recorded
    )

    # Install headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )

    # Export target definitions
    install(EXPORT MyUITargets
        FILE MyUITargets.cmake
        NAMESPACE MyUI::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )

    # Generate and install config + version files
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MyUIConfigVersion.cmake"
        VERSION ${MYUI_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MyUIConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/MyUIConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/MyUIConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/MyUIConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyUI
    )
endif()

# ---- Example ----
if(MYUI_BUILD_EXAMPLES)
    add_executable(SimpleButton examples/main.cpp)
    target_link_libraries(SimpleButton PRIVATE MyUI)
endif()

# ---- Compile Commands ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

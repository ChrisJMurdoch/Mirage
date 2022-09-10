
# ===== PARAMETERS =====

# CMake version
cmake_minimum_required(VERSION 3.23)

# Project
project("WorldEngine" VERSION 0.2.0)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)

# ===== VENDOR RESOURCE PATHS =====

set(GLAD_SRC     vendor/glad-4.6-core/src)
set(GLAD_INCLUDE vendor/glad-4.6-core/include)

set(GLFW_INCLUDE vendor/glfw-3.3.8/include)
set(GLFW_BIN     vendor/glfw-3.3.8/bin/x64/debug)

# ===== BUILDSCRIPT =====

# --- Render Library ---

add_library(Render STATIC)
target_sources(Render
    PRIVATE # Internal
        Render/src/display/display.cpp
    PRIVATE # Vendor
        ${GLAD_SRC}/glad.c
)
target_include_directories(Render
    PRIVATE # Internal
        Render/include/
    PRIVATE # Vendor
        ${GLAD_INCLUDE}
        ${GLFW_INCLUDE}
)

# --- Redshift Application ---

add_executable(Redshift)
target_sources(Redshift
    PRIVATE # Internal
        Redshift/src/redshift/main.cpp
)
target_include_directories(Redshift
    PRIVATE # Internal
        Redshift/include/
    PRIVATE # Project
        Render/include/
)
target_link_libraries(Redshift
    PRIVATE # Project
        Render
    PRIVATE # Vendor
        ${CMAKE_SOURCE_DIR}/${GLFW_BIN}/glfw3.lib
)

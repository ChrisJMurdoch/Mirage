
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

set(GLAD_INCLUDE vendor/glad/include)
set(GLAD_SRC     vendor/glad/src)

set(GLFW_BIN     vendor/glfw/bin)
set(GLFW_INCLUDE vendor/glfw/include)

set(GLM_INCLUDE  vendor/glm/include)

# ===== BUILDSCRIPT =====

# --- Utility Library ---

add_library(Utility STATIC)
target_sources(Utility
    PRIVATE # Internal
        Utility/src/utility/debug.cpp
        Utility/src/utility/io.cpp
)
target_include_directories(Utility
    PRIVATE # Internal
        Utility/include/
)

# --- Generate Library ---

add_library(Generate STATIC)
target_sources(Generate
    PRIVATE # Internal source files
        Generate/src/generate/noise.cpp
)
target_include_directories(Generate
    PRIVATE # Internal
        Generate/include/
    PRIVATE # Project
        Utility/include/
    PRIVATE # Vendor
        ${GLM_INCLUDE}
)

# --- Render Library ---

add_library(Render STATIC)
target_sources(Render
    PRIVATE # Internal
        Render/src/arrayBuffer/vertexBuffer.cpp
        Render/src/model/instance.cpp
        Render/src/model/model.cpp
        Render/src/render/display.cpp
        Render/src/render/program.cpp
)
target_include_directories(Render
    PRIVATE # Internal
        Render/include/
    PRIVATE # Project
        Utility/include/
    PRIVATE # Vendor
        ${GLAD_INCLUDE}
        ${GLFW_INCLUDE}
        ${GLM_INCLUDE}
)

# --- Hex Executable ---

add_executable(Hex)
target_sources(Hex
    PRIVATE # Internal
        Hex/src/hexEngine/hexCell.cpp
        Hex/src/hexEngine/main.cpp
    PRIVATE # Vendor
        ${GLAD_SRC}/glad.c
)
target_include_directories(Hex
    PRIVATE # Internal
        Hex/include/
    PRIVATE # Project
        Generate/include/
        Render/include/
    PRIVATE # Vendor
        ${GLAD_INCLUDE}
        ${GLFW_INCLUDE}
        ${GLM_INCLUDE}
)
target_link_libraries(Hex
    PRIVATE # Project
        Utility
        Generate
        Render
    PRIVATE # Vendor
        ${CMAKE_SOURCE_DIR}/vendor/glfw/bin/glfw3.lib
)

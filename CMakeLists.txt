
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

set(GLM_INCLUDE  vendor/glm-9.9.9.8/include)

set(STBI_INCLUDE vendor/stb_image/include)

# ===== BUILDSCRIPT =====

# --- Utility Library ---

add_library(Utility STATIC)
target_sources(Utility
    PRIVATE # Internal
        Utility/src/utility/objLoader.cpp
)
target_include_directories(Utility
    PRIVATE # Internal
        Utility/include/
    PRIVATE # Vendor
        ${GLM_INCLUDE}
)

# --- Render Library ---

add_library(Render STATIC)
target_sources(Render
    PRIVATE # Internal
    Render/src/display/display.cpp
    Render/src/display/model.cpp
    Render/src/display/mtlLoader.cpp
    Render/src/display/program.cpp
    Render/src/display/texture.cpp
    PRIVATE # Vendor
        ${GLAD_SRC}/glad.c
)
target_include_directories(Render
    PRIVATE # Internal
        Render/include/
        Render/src/ # For .inl files
    PRIVATE # Project
        Utility/include/
    PRIVATE # Vendor
        ${GLAD_INCLUDE}
        ${GLFW_INCLUDE}
        ${GLM_INCLUDE}
        ${STBI_INCLUDE}
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
        Utility/include/
        Render/include/
    PRIVATE # Vendor
        ${GLM_INCLUDE}
)
target_link_libraries(Redshift
    PRIVATE # Project
        Utility
        Render
    PRIVATE # Vendor
        ${CMAKE_SOURCE_DIR}/${GLFW_BIN}/glfw3.lib
)

# --- Aether Application ---

add_executable(Aether)
target_sources(Aether
    PRIVATE # Internal
        Aether/src/aether/image.cpp
        Aether/src/aether/kdtree.cpp
        Aether/src/aether/main.cpp
        Aether/src/aether/rayGeometry.cpp
        Aether/src/aether/rayscene.cpp
)
target_include_directories(Aether
    PRIVATE # Internal
        Aether/include/
        Aether/src/ # For .inl files
    PRIVATE # Project
        Utility/include/
        Render/include/
    PRIVATE # Vendor
        ${GLM_INCLUDE}
        ${STBI_INCLUDE}
)
target_link_libraries(Aether
    PRIVATE # Project
        Utility
        Render
    PRIVATE # Vendor
        ${CMAKE_SOURCE_DIR}/${GLFW_BIN}/glfw3.lib
)

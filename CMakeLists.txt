
# ===== PARAMETERS =====

# CMake version
cmake_minimum_required(VERSION 3.23)

# Project
project("Mirage" VERSION 0.3.0)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

# ===== VENDOR RESOURCE PATHS =====

set(FBXSDK_INCLUDE vendor/fbxsdk-2020.3.4/include)
set(FBXSDK_BIN     vendor/fbxsdk-2020.3.4/bin)

set(GLAD_SRC       vendor/glad-4.6-core/src)
set(GLAD_INCLUDE   vendor/glad-4.6-core/include)

set(GLFW_INCLUDE   vendor/glfw-3.3.8/include)
set(GLFW_BIN       vendor/glfw-3.3.8/bin/x64/release)

set(GLM_INCLUDE    vendor/glm-9.9.9.8/include)

set(STBI_INCLUDE   vendor/stb_image/include)

# ===== BUILDSCRIPT =====

# --- Mirage Application ---

add_executable(Mirage)
target_sources(Mirage
    PRIVATE # Internal
        Mirage/src/aether/aether.cpp
        Mirage/src/aether/image.cpp
        Mirage/src/aether/kdtree.cpp
        Mirage/src/aether/raytrace.cpp
        Mirage/src/mirage/main.cpp
        Mirage/src/render/display.cpp
        Mirage/src/render/model.cpp
        Mirage/src/render/mtlLoader.cpp
        Mirage/src/render/program.cpp
        Mirage/src/render/texture.cpp
        Mirage/src/render/display.cpp
        Mirage/src/render/model.cpp
        Mirage/src/render/mtlLoader.cpp
        Mirage/src/render/program.cpp
        Mirage/src/render/texture.cpp
        Mirage/src/utility/fbxLoader.cpp
        Mirage/src/utility/objLoader.cpp
    PRIVATE # Vendor
        ${GLAD_SRC}/glad.c
)
target_include_directories(Mirage
    PRIVATE # Internal
        Mirage/include/
        Mirage/inline/
    PRIVATE # Vendor
        ${FBXSDK_INCLUDE}
        ${GLAD_INCLUDE}
        ${GLFW_INCLUDE}
        ${GLM_INCLUDE}
        ${STBI_INCLUDE}
)
target_link_libraries(Mirage
    PRIVATE # Vendor
        ${CMAKE_SOURCE_DIR}/${GLFW_BIN}/glfw3.lib
        ${CMAKE_SOURCE_DIR}/${FBXSDK_BIN}/libfbxsdk-mt.lib
        ${CMAKE_SOURCE_DIR}/${FBXSDK_BIN}/libxml2-mt.lib
        ${CMAKE_SOURCE_DIR}/${FBXSDK_BIN}/zlib-mt.lib
)

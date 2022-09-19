
#pragma once

// PARAMETERS
#define ENABLE_CACHING 1         // Injects a runtime check to ignore duplicate calls to state-setting gl functions
#define ENABLE_UNBIND_OMISSION 0 // Injects a runtime check to ignore calls to unbind objects

/** Abstractions over specific OpenGL function calls to avoid redundant state changes */
namespace gl
{
    static inline void useProgram(unsigned int handle);
    static inline void bindVertexArray(unsigned int handle);
    static inline void bindArrayBuffer(unsigned int handle);
    static inline void bindElementArrayBuffer(unsigned int handle);
    static inline void activeTexture(unsigned int handle);
    static inline void bindTexture2D(unsigned int handle);
}

#include "display/gl.inl"

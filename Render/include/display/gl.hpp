
#pragma once

#include <glad/glad.h>

// PARAMETERS
#define ENABLE_CACHING 1         // Injects a runtime check to ignore duplicate calls to state-setting gl functions
#define ENABLE_UNBIND_OMISSION 0 // Injects a runtime check to ignore calls to unbind objects

// Caching
#if ENABLE_CACHING!=0
    #define CACHE_ESCAPE static unsigned int cachedHandle=0; if (handle==cachedHandle) return; cachedHandle=handle;
#else
    #define CACHE_ESCAPE
#endif

// Unbind omission
#if ENABLE_UNBIND_OMISSION!=0
    #define UNBIND_ESCAPE if (handle==0) return;
#else
    #define UNBIND_ESCAPE
#endif

/** Abstractions over specific OpenGL function calls to avoid redundant state changes */
namespace gl
{
    static inline void useProgram(unsigned int handle)             { UNBIND_ESCAPE CACHE_ESCAPE glUseProgram(handle);                          }
    static inline void bindVertexArray(unsigned int handle)        { UNBIND_ESCAPE CACHE_ESCAPE glBindVertexArray(handle);                     }
    static inline void bindArrayBuffer(unsigned int handle)        { UNBIND_ESCAPE CACHE_ESCAPE glBindBuffer(GL_ARRAY_BUFFER, handle);         }
    static inline void bindElementArrayBuffer(unsigned int handle) { UNBIND_ESCAPE CACHE_ESCAPE glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle); }
}

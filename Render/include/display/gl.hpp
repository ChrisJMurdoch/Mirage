
#pragma once

#include <glad/glad.h>

#define DISABLE_CACHING 0

/** Abstraction over specific OpenGL function calls to avoid redundant state changes */
namespace gl
{
    // Template state-caching functions so they can be constructed and potentially inlined at compile time
    #if DISABLE_CACHING == 1
        #define cachedCall(glFunction,functionName) static inline void functionName(unsigned int handle)    \
        {                                                                                                   \
            glFunction(handle);                                                                             \
        }
        #define boundArgCachedCall(glFunction,boundArg,functionName) static inline void functionName(unsigned int handle)   \
        {                                                                                                                   \
            glFunction(boundArg,handle);                                                                                    \
        }
    #else
        #define cachedCall(glFunction,functionName) static inline void functionName(unsigned int handle)    \
        {                                                                                                   \
            static int cachedHandle = 0;                                                                    \
            if (handle!=cachedHandle)                                                                       \
                glFunction(cachedHandle=handle);                                                            \
        }
        #define boundArgCachedCall(glFunction,boundArg,functionName) static inline void functionName(unsigned int handle)   \
        {                                                                                                                   \
            static int cachedHandle = 0;                                                                                    \
            if (handle!=cachedHandle)                                                                                       \
                glFunction(boundArg,cachedHandle=handle);                                                                   \
        }
    #endif

    // Create template instances
    cachedCall(glUseProgram, useProgram);
    cachedCall(glBindVertexArray, bindVertexArray);
    boundArgCachedCall(glBindBuffer, GL_ARRAY_BUFFER, bindArrayBuffer);
    boundArgCachedCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, bindElementArrayBuffer);
}

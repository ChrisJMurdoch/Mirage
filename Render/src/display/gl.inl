
#include "display/gl.hpp"

#include <glad/glad.h>

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

// Allow caching - bound to global instance
static inline void gl::useProgram(unsigned int handle)             { UNBIND_ESCAPE CACHE_ESCAPE glUseProgram(handle);                          }
static inline void gl::bindVertexArray(unsigned int handle)        { UNBIND_ESCAPE CACHE_ESCAPE glBindVertexArray(handle);                     }
static inline void gl::bindArrayBuffer(unsigned int handle)        { UNBIND_ESCAPE CACHE_ESCAPE glBindBuffer(GL_ARRAY_BUFFER, handle);         }
static inline void gl::bindElementArrayBuffer(unsigned int handle) { UNBIND_ESCAPE CACHE_ESCAPE glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle); }
static inline void gl::activeTexture(unsigned int handle)          { UNBIND_ESCAPE CACHE_ESCAPE glActiveTexture(handle);                       }

// Disallow caching - binding environment can change
static inline void gl::bindTexture2D(unsigned int handle)          { UNBIND_ESCAPE glBindTexture(GL_TEXTURE_2D, handle); }

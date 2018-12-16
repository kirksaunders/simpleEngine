#ifndef BUFFERCONTAINERS_HPP
#define BUFFERCONTAINERS_HPP

#include <GLEW/glew.h>

namespace Render3D {
    struct BufferPair {
        GLuint first;
        GLuint second;
        GLuint useCount;

        BufferPair() : first(0), second(0), useCount(1) {}
        BufferPair(GLuint&& f, GLuint&& s) : first(f), second(s), useCount(1) {}
        BufferPair(const GLuint& f, const GLuint& s) : first(f), second(s), useCount(1) {}
    };

    struct BufferTriple {
        GLuint first;
        GLuint second;
        GLuint third;
        GLuint useCount;

        BufferTriple() : first(0), second(0), third(0), useCount(1) {}
        BufferTriple(GLuint&& f, GLuint&& s, GLuint&& t) : first(f), second(s), third(t), useCount(1) {}
        BufferTriple(const GLuint& f, const GLuint& s, const GLuint& t) : first(f), second(s), third(t), useCount(1) {}
    };
};

#endif
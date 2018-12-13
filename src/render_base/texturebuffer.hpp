#ifndef TEXTUREBUFFER_HPP
#define TEXTUREBUFFER_HPP

#define GLEW_STATIC

#include <GLEW/glew.h>

namespace Render3D {
    class Color; // forward declaration

    class TextureBuffer {
     public:
        TextureBuffer(size_t w, size_t h, size_t d);
        TextureBuffer(const TextureBuffer &buff);
        ~TextureBuffer();

        void setValue(size_t x, size_t y, size_t z, GLubyte value);
        void setPixel(size_t x, size_t y, const Color& col);

        size_t getWidth() const;
        size_t getHeight() const;
        size_t getDepth() const;

        GLubyte* getAddress();
        const GLubyte* const getAddress() const;

     private:
        size_t width;
        size_t height;
        size_t depth;

        GLubyte* data;
    };
}

#endif
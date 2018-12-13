#include <algorithm>

#include "render_base/texturebuffer.hpp"
#include "render_base/color.hpp"

using namespace Render3D;

TextureBuffer::TextureBuffer(size_t w, size_t h, size_t d) {
    width = w;
    height = h;
    depth = d;

    data = new GLubyte[height * width * depth];
}

TextureBuffer::TextureBuffer(const TextureBuffer &buff) {
    width = buff.width;
    height = buff.height;
    depth = buff.depth;

    data = new GLubyte[height * width * depth];
    std::copy(buff.data, buff.data + (height * width * depth), data);
}

TextureBuffer::~TextureBuffer() {
    delete[] data;
}

void TextureBuffer::setValue(size_t x, size_t y, size_t z, GLubyte value) {
    data[y*width*depth + x*depth + z] = value;  //  x*height*depth + y*depth + z
}

void TextureBuffer::setPixel(size_t x, size_t y, const Color& col) {
    if (depth != 3 && depth != 4) {
        // error
        return;
    }
    size_t index = y*width*depth + x*depth;

    data[index] = col.getRed() *  255;
    data[index + 1] = col.getGreen() *  255;
    data[index + 2] = col.getBlue() * 255;
    if (depth == 4) {
        data[index + 3] = col.getAlpha() * 255;
    }
}

size_t TextureBuffer::getWidth() const {
    return width;
}

size_t TextureBuffer::getHeight() const {
    return height;
}

size_t TextureBuffer::getDepth() const {
    return depth;
}

GLubyte* TextureBuffer::getAddress() {
    return data;
}

const GLubyte* const TextureBuffer::getAddress() const {
    return data;
}
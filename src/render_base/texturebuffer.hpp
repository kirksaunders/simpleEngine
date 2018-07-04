#ifndef TEXTUREBUFFER_HPP
#define TEXTUREBUFFER_HPP

#define GLEW_STATIC

#include <GLEW/glew.h>

#include "render_base/color.hpp"

namespace Render3D {
	class TextureBuffer {
	 public:
		TextureBuffer(size_t w, size_t h, size_t d);
		TextureBuffer(const TextureBuffer &buff);
		~TextureBuffer();

		void setValue(size_t x, size_t y, size_t z, GLubyte value);
		void setPixel(size_t x, size_t y, Color col);

		GLubyte* getAddress();

	 private:
		size_t width;
		size_t height;
		size_t depth;

		GLubyte* data;
	};
}

#endif
#ifndef BUFFEROBJECT_HPP
#define BUFFEROBJECT_HPP

#include <GLEW/glew.h>

namespace Render3D {
	struct BufferObject {
		GLuint id;
		GLuint useCount;

		BufferObject() : id(0), useCount(0) {}
	};

	struct DoubleBufferObject {
		GLuint firstID;
		GLuint secondID;
		GLuint useCount;

		DoubleBufferObject() : firstID(0), secondID(0), useCount(0) {}
	};
};

#endif
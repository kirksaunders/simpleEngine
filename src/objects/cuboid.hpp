#ifndef CUBOID2_HPP
#define CUBOID2_HPP

#define GLEW_STATIC

#include <vector>
#include <unordered_map>

#include <GLEW/glew.h>

#include "render_base/shader.hpp"
#include "objects/primitive3d.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	class Cuboid : public Primitive3D {
	 public:
		Cuboid();

		int getVertexCount();

		void render(Shader& shader);

	 private:
		GLfloat vertices[108];
		GLfloat normals[108];
		GLushort vertexIndices[36];
		GLushort normalIndices[36];
		GLuint VBO;
		GLuint NBO;

		std::unordered_map<Window*, GLuint> VAOs;

		void setNormals();

		void setVertices();

		void generateBuffers();
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
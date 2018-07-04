#ifndef CUBOID2_HPP
#define CUBOID2_HPP

#define GLEW_STATIC

#include <vector>
#include <map>

#include <GLEW/glew.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
//#include "render_base/face.hpp"
#include "render_base/shader.hpp"
#include "objects/primitive3d.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	class Cuboid : public Primitive3D {
	 public:
		Cuboid();

		//std::vector<Face> getFaces();

		//GLuint getVertexArrayObject();

		int getVertexCount();

		void render(Shader& shader);

	 private:
		GLfloat vertices[108];
		GLfloat normals[108];
		GLushort vertexIndices[36];
		GLushort normalIndices[36];
		GLuint VBO;
		GLuint NBO;

		std::map<Window*, GLuint> VAOs;

		//std::vector<Face> faces;

		//void calculateFaces();

		void setNormals();

		void setVertices();

		void generateBuffers();
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
#ifndef CUBOID2_HPP
#define CUBOID2_HPP

#define GLEW_STATIC

#include <unordered_map>

#include <GLEW/glew.h>

#include "math/vector4.hpp"

#include "objects/primitive3d.hpp"

namespace Render3D {
	class Cuboid : public Primitive3D {
	 public:
		Cuboid();

		int getVertexCount();

		void render(Shader* const shader, Window* const win, TextureManager* const textureManager);

        void prepareContent(Window* win, TextureManager* textureManager);

	 private:
        typedef std::pair<GLuint, GLuint> BufferPair;
        std::unordered_map<GLuint, BufferPair> BufferObjects;
		std::unordered_map<Window*, GLuint> VAOs;

        Math3D::Vector4 vertices[8];
        Math3D::Vector4 normals[6];
        GLushort vertexIndices[36];
        GLushort normalIndices[36];

		void setNormals();
		void setVertices();
        void loadData();

		void generateBuffers(GLuint clusterID);
        void generateVertexArrayObject(GLuint clusterID, Window* win);
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <utility>
#include <vector>

#include <GLEW/glew.h>

#include "math/vector4.hpp"

#include "objects/bufferobject.hpp"
#include "objects/primitive3d.hpp"

namespace Render3D {
    class Cuboid : public Primitive3D {
     public:
        Cuboid();

        void render(Window& win, TextureManager& textureManager);

        void prepareContent(Window& win, TextureManager& textureManager);
        void destroyContent(Window& win, TextureManager& textureManager);

     private:
        std::vector<std::pair<GLuint, BufferObject> > bufferObjects;
        std::vector<std::pair<const Window*, GLuint> > VAOs;

		Math3D::Vector4 vertices[8];
		Math3D::Vector4 normals[6];
		GLushort vertexIndices[36];
		GLushort normalIndices[36];

        void setNormals();
        void setVertices();
        void loadData();

        BufferObject generateBuffers(GLuint clusterID);
        void destroyBuffers(GLuint clusterID);
        void generateVertexArrayObject(Window& win);
        void destroyVertexArrayObject(Window& win);
        GLuint getVertexArrayObject(Window& win);
    };
}

#endif
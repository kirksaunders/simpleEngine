#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <utility>
#include <vector>

#include "math/vector4.hpp"

#include "objects/bufferobject.hpp"
#include "objects/primitive3d.hpp"

#include "render_base/texture.hpp"

namespace Render3D {
    struct TextureCoord {
        GLfloat x;
        GLfloat y;
    };

    struct TextureData {
        std::string type;
        Texture* tex;

        TextureData() : type(""), tex(nullptr) {}
        TextureData(const std::string& typ, Texture* t) : type(typ), tex(t) {}
    };

    class Mesh {
     public:
        Mesh(const std::vector<Math3D::Vector4>& verts, const std::vector<Math3D::Vector4>& norms,
             const std::vector<TextureCoord>& texCs, const std::vector<TextureData>& texs,
             const std::vector<GLuint>& inds);

        void bindTextures(Shader& shader, Window& win, TextureManager& textureManager);

        void unbindTextures(Shader& shader, Window& win, TextureManager& textureManager);

        int getVertexCount();

        void render(Shader& shader, Window& win, TextureManager& textureManager);
        void prepareContent(Window& win, TextureManager& textureManager);
        void destroyContent(Window& win, TextureManager& textureManager);

     private:
        std::vector<Math3D::Vector4> vertices;
        std::vector<Math3D::Vector4> normals;
        std::vector<TextureCoord> texCoords;
        std::vector<TextureData> textures;
        std::vector<GLuint> indices;

        unsigned int numVertices;

        std::vector<std::pair<GLuint, DoubleBufferObject>> bufferObjects;
        std::vector<std::pair<const Window*, GLuint>> VAOs;

        DoubleBufferObject generateBuffers(GLuint clusterID);
        void destroyBuffers(GLuint clusterID);
        void generateVertexArrayObject(Window& win);
        void destroyVertexArrayObject(Window& win);
        GLuint getVertexArrayObject(Window& win);
        GLuint getElementArrayBuffer(GLuint clusterID);
    };
}

#endif
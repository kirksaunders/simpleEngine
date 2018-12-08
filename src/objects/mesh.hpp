#ifndef MESH_HPP
#define MESH_HPP

#define GLEW_STATIC

#include <vector>
#include <string>
#include <utility>

#include "objects/buffercontainers.hpp"
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

		void bindTextures(Shader& shader, const Window& win, TextureManager& textureManager);

		void unbindTextures(Shader& shader, const Window& win, TextureManager& textureManager);

		int getVertexCount();

		void render(Shader& shader, const Window& win, TextureManager& textureManager);
        void prepareContent(const Window& win, TextureManager& textureManager);
        void destroyContent(const Window& win, TextureManager& textureManager);

	 private:
        std::vector<Math3D::Vector4> vertices;
        std::vector<Math3D::Vector4> normals;
        std::vector<TextureCoord> texCoords;
        std::vector<TextureData> textures;
        std::vector<GLuint> indices;

        unsigned int numVertices;

        std::vector<std::pair<GLuint, BufferTriple> > bufferObjects;
		std::vector<std::pair<const Window*, GLuint> > VAOs;

        BufferTriple generateBuffers(GLuint clusterID);
        void destroyBuffers(GLuint clusterID);
        void generateVertexArrayObject(const Window& win);
        void destroyVertexArrayObject(const Window& win);
		GLuint getVertexArrayObject(const Window& win);
	};
}

#endif
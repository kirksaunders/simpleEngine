#ifndef MESH_HPP
#define MESH_HPP

#define GLEW_STATIC

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <utility>

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

		TextureData() : type(""), tex(NULL) {}
		TextureData(const std::string& typ, Texture* t) : type(typ), tex(t) {}
	};

	class Mesh {
	 public:
		Mesh(const std::vector<Math3D::Vector4>& verts, const std::vector<Math3D::Vector4>& norms,
             const std::vector<TextureCoord>& texCs, const std::vector<TextureData>& texs,
             const std::vector<GLuint>& inds);

		void bindTextures(Shader* shader, Window* win, TextureManager* textureManager);

		void unbindTextures(Shader* shader, Window* win, TextureManager* textureManager);

		int getVertexCount();

		void render(Shader* const shader, Window* const win, TextureManager* const textureManager);
        void prepareContent(Window* win, TextureManager* textureManager);

	 private:
        std::vector<Math3D::Vector4> vertices;
        std::vector<Math3D::Vector4> normals;
        std::vector<TextureCoord> texCoords;
        std::vector<TextureData> textures;
        std::vector<GLuint> indices;

        unsigned int numVertices;

        typedef std::tuple<GLuint, GLuint, GLuint> BufferTriple;
        std::unordered_map<GLuint, BufferTriple> BufferObjects;
		std::unordered_map<Window*, GLuint> VAOs;

        void generateBuffers(GLuint clusterID);
        void generateVertexArrayObject(GLuint clusterID, Window* win);
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
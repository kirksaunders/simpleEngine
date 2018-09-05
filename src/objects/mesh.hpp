#ifndef MESH_HPP
#define MESH_HPP

#define GLEW_STATIC

#include <vector>
#include <string>

#include <GLEW/glew.h>
#include <ASSIMP/scene.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
#include "render_base/shader.hpp"
#include "render_base/texturemanager.hpp"
#include "objects/primitive3d.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	struct TextureCoord {
		GLfloat x;
		GLfloat y;
	};

	struct TextureData {
		GLuint id;
		std::string type;
		aiString path;
		std::string fullPath;
	};

	class Mesh {
	 public:
		Mesh(const std::vector<Math3D::Vector4>& vertices, const std::vector<Math3D::Vector4>& normals,
                const std::vector<TextureCoord>& texCoords, const std::vector<TextureData>& textures,
                const std::vector<GLuint>& indices);

		void bindTextures(Shader& shader);

		void unbindTextures(Shader& shader);

		int getVertexCount();

		void render(Shader& shader);

	 private:
        unsigned int numVertices;
		std::string directory;

		std::vector<TextureData> textures;

		GLuint VBO;
		GLuint NBO;
		GLuint TBO;

		std::map<Window*, GLuint> VAOs;

		void processNode(aiNode* node, const aiScene* scene);

		void processMesh(aiMesh* mesh, const aiScene* scene);

		void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
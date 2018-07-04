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
		Mesh(std::vector<Math3D::Vector4> vertices, std::vector<Math3D::Vector4> normals, std::vector<TextureCoord> texCoords, std::vector<TextureData> textures, std::vector<GLuint> indices);

		//std::vector<Face> getFaces();

		//GLuint getVertexArrayObject();

		void bindTextures(Shader& shader);

		void unbindTextures(Shader& shader);

		int getVertexCount();

		void render(Shader& shader);

	 private:
		std::string directory;

		std::vector<Math3D::Vector4> vertices;
		std::vector<Math3D::Vector4> normals;
		std::vector<TextureCoord> texCoords;
		std::vector<TextureData> textures;
		std::vector<GLuint> indices;
		GLuint VBO;
		GLuint NBO;
		GLuint TBO;

		std::map<Window*, GLuint> VAOs;

		void updateVertices();

		void updateNormals();

		void processNode(aiNode* node, const aiScene* scene);

		void processMesh(aiMesh* mesh, const aiScene* scene);

		void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		void generateBuffers();
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
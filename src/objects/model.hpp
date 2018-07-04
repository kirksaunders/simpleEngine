#ifndef MODEL_HPP
#define MODEL_HPP

#define GLEW_STATIC

#include <vector>
#include <string>

#include <GLEW/glew.h>
#include <ASSIMP/scene.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
#include "render_base/shader.hpp"
#include "objects/mesh.hpp"
#include "objects/primitive3d.hpp"

#include "render_base/texturemanager.hpp"
#include "render_base/texture.hpp"

namespace Render3D {
	class Model : public Primitive3D {
	 public:
		Model() {}

		Model(char const *filePath);

		//int getVertexCount();

		void bindTextures(Shader& shader, TextureManager& textureManager);

		void render(Shader& shader);

	 private:
		std::string directory;

		std::vector<TextureData> texturesLoaded;

		std::vector<Mesh> meshes;

		void updateVertices();

		void updateNormals();

		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}

#endif
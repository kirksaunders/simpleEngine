#ifndef MODEL_HPP
#define MODEL_HPP

#define GLEW_STATIC

#include <vector>
#include <string>
#include <unordered_map>

#include <GLEW/glew.h>
#include <ASSIMP/scene.h>

#include "objects/primitive3d.hpp"
#include "objects/mesh.hpp"

#include "render_base/texture.hpp"

namespace Render3D {
	class Model : public Primitive3D {
	 public:
		Model() : Primitive3D() {}

		Model(const char *filePath);

		void render(const Window& win, TextureManager& textureManager);

        void prepareContent(const Window& win, TextureManager& textureManager);
        void destroyContent(const Window& win, TextureManager& textureManager);

	 private:
		std::string directory;
        typedef std::unordered_map<std::string, Texture> TexturesMap;
        TexturesMap textureCache;

		std::vector<Mesh> meshes;

		void updateVertices();

		void updateNormals();

		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureData>& textures);
	};
}

#endif
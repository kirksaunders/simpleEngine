#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <utility>
#include <vector>

#include <GLEW/glew.h>
#include <ASSIMP/scene.h>

#include "objects/mesh.hpp"
#include "objects/primitive3d.hpp"

#include "render_base/texture.hpp"

namespace Render3D {
    class Model : public Primitive3D {
     public:
        Model() : Primitive3D() {}
        ~Model();

        Model(const char *filePath);

        void render(Window& win, TextureManager& textureManager);

        void prepareContent(Window& win, TextureManager& textureManager);
        void destroyContent(Window& win, TextureManager& textureManager);

     private:
        std::string directory;
        std::vector<std::pair<std::string, Texture*> > textureCache;

        std::vector<Mesh> meshes;

        void updateVertices();

        void updateNormals();

        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureData>& textures);
    };
}

#endif
#ifndef CONTEXT3D_HPP
#define CONTEXT3D_HPP

#include <memory>
#include <set>

#include "render_base/camera.hpp"
#include "render_base/shadervariableblock.hpp"
#include "render_base/texturemanager.hpp"
#include "render_base/uniformbuffermanager.hpp"

namespace Render3D {
    // forward declarations
    class Primitive3D;
    class Shader;
    class Texture;
    class Window;

    class Context3D {
     public:
        Context3D();
        Context3D(Window* win);
        ~Context3D();

        void addObject(Primitive3D* object);
        void removeObject(Primitive3D* object);
        void clearObjects();

        void addShader(Shader* shader);
        void removeShader(Shader* Shader);
        void clearShaders();

        void addTexture(Texture* texture);
        void removeTexture(Texture* texture);
        void clearTextures();

        Camera* const getCamera();
        TextureManager* const getTextureManager();

        void render();

        void renderTexture(Texture& tex);

     private:
        Window* window;
        std::shared_ptr<TextureManager> textureManager;
        UniformBufferManager uniformBufferManager;

        std::set<Primitive3D*> objects;
        std::set<Shader*> shaders;
        std::set<Texture*> textures;

        Camera camera;

        ShaderVariableBlock testBlock;
    };
}

#endif
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

        void addObject(std::shared_ptr<Primitive3D> object);
        void removeObject(std::shared_ptr<Primitive3D>& object);
        void clearObjects();

        void addShader(std::shared_ptr<Shader> shader);
        void removeShader(std::shared_ptr<Shader>& Shader);
        void clearShaders();

        void addTexture(std::shared_ptr<Texture> texture);
        void removeTexture(std::shared_ptr<Texture>& texture);
        void clearTextures();

        Camera* const getCamera();
        TextureManager* const getTextureManager();

        void render();

        void renderTexture(std::shared_ptr<Texture> tex);

     private:
        Window* window;
        std::shared_ptr<TextureManager> textureManager;
        UniformBufferManager uniformBufferManager;

        std::set<std::shared_ptr<Primitive3D>> objects;
        std::set<std::shared_ptr<Shader>> shaders;
        std::set<std::shared_ptr<Texture>> textures;

        Camera camera;

        ShaderVariableBlock testBlock;
    };
}

#endif
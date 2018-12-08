#ifndef CONTEXT3D_HPP
#define CONTEXT3D_HPP

#include <set>
#include <memory>

#include "render_base/camera.hpp"
#include "render_base/shaderManager.hpp"
#include "render_base/textureManager.hpp"

namespace Render3D {
    // forward declarations
    class Window;
    class Texture;
    class Primitive3D;
    class Shader;

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

		std::set<Primitive3D*> objects;
		std::set<Shader*> shaders;
		std::set<Texture*> textures;

		Camera camera;
	};
}

#endif
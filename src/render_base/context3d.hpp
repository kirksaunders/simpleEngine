#ifndef CONTEXT3D_HPP
#define CONTEXT3D_HPP

#include <vector>
#include <memory>

#include "render_base/camera.hpp"
#include "render_base/shaderManager.hpp"
#include "render_base/textureManager.hpp"

namespace Render3D {
    // forward declarations
    class Window;
    class Texture;
    class Primitive3D;

	class Context3D {
	 public:
		Context3D();
		Context3D(Window* win);
        ~Context3D();

		void addObject(Primitive3D* object);

		Camera* getCamera();
		TextureManager* getTextureManager();

		void render();

		void renderTexture(Texture& tex);

	 private:
		Window* window;
        std::shared_ptr<ShaderManager> shaderManager;
        std::shared_ptr<TextureManager> textureManager;

		std::vector<Primitive3D*> objects;

		Camera camera;
	};
}

#endif
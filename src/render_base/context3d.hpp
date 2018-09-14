#ifndef CONTEXT3D_HPP
#define CONTEXT3D_HPP

#include <vector>

#include "render_base/window.hpp"
#include "render_base/camera.hpp"
#include "objects/primitive3d.hpp"
#include "render_base/texture.hpp"

namespace Render3D {
	class Context3D {
	 public:
		Context3D();

		Context3D(Window* win);

		void addObject(Primitive3D* object);

		Camera* getCamera();

		void render();

		void renderTexture(Texture& tex);

	 private:
		Window* window;

		std::vector<Primitive3D*> objects;

		Camera camera;
	};
}

#endif
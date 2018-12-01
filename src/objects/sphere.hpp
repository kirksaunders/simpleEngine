#ifndef SPHERE_HPP
#define SPHERE_HPP

#define GLEW_STATIC

#include <GLEW/glew.h>

#include "objects/primitive3d.hpp"
#include "objects/model.hpp"

namespace Render3D {
	class Sphere : public Primitive3D {
	 public:
		Sphere();

		void setSize(const Math3D::Vector4& value);

		void setCFrame(const Math3D::Matrix4x4& value);

		void setColor(const Color& value);

		void render(Shader* const shader, Window* const win, TextureManager* const textureManager);
        void prepareContent(Window* win, TextureManager* textureManager);
	 private:
		Model model;
	};
}

#endif
#ifndef SPHERE_HPP
#define SPHERE_HPP

#define GLEW_STATIC

#include <vector>

#include <GLEW/glew.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
#include "objects/face.hpp"
#include "render_base/shader.hpp"
#include "objects/primitive3d.hpp"
#include "objects/model.hpp"

namespace Render3D {
	class Sphere : public Primitive3D {
	 public:
		Sphere();

		void setSize(const Math3D::Vector4& value);

		Math3D::Vector4 getSize() const;

		void setCFrame(const Math3D::Matrix4x4& value);

		Math3D::Matrix4x4 getCFrame() const;

		void setColor(const Color& value);

		Color getColor() const;

		void setShader(char const *name);

		char const* getShader() const;

		void render(Shader& shader);
	 private:
		Model model;
		GLuint VAO;
		GLuint VBO;
		GLuint NBO;
	};
}

#endif
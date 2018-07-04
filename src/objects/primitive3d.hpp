#ifndef PRIMITIVE3D_HPP
#define PRIMITIVE3D_HPP

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
#include "render_base/shader.hpp"

namespace Render3D {
	class Primitive3D { // Primitive3D is an abstract class
	 public:
		Primitive3D();
        virtual ~Primitive3D() = 0;

		virtual void setSize(const Math3D::Vector4& value);

		virtual Math3D::Vector4 getSize() const;

		virtual void setCFrame(const Math3D::Matrix4x4& value);

		virtual Math3D::Matrix4x4 getCFrame() const;

		virtual void setColor(const Color& value);

		virtual Color getColor() const;

		virtual void setShader(char const *name);

		virtual char const* getShader() const;

		virtual void render(Shader& shader) = 0;

	 protected:
		char const *shaderName;
		Math3D::Vector4 size;
		Math3D::Matrix4x4 cframe;
		Color color;
	};
}

#endif
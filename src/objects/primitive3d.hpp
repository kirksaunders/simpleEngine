#ifndef PRIMITIVE3D_HPP
#define PRIMITIVE3D_HPP

#include <string>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"
#include "render_base/shadervariable.hpp"

namespace Render3D {
    // forward declarations
    class Window;
    class Shader;
    class TextureManager;

	class Primitive3D { // Primitive3D is an abstract class
	 public:
		Primitive3D();
		virtual ~Primitive3D() {};

		virtual void setSize(const Math3D::Vector4& value);

		virtual Math3D::Vector4 getSize() const;

		virtual void setCFrame(const Math3D::Matrix4x4& value);

		virtual Math3D::Matrix4x4 getCFrame() const;

		virtual void setColor(const Color& value);

		virtual Color getColor() const;

		virtual void setShader(Shader* const s);

		virtual Shader* const getShader() const;

		virtual void render(const Window& win, TextureManager& textureManager) = 0;

        virtual void prepareContent(const Window& win, TextureManager& textureManager) = 0;
        virtual void destroyContent(const Window& win, TextureManager& textureManager) = 0;

	 protected:
		Shader* shader;
		Math3D::Vector4 size;
		Math3D::Matrix4x4 cframe;
		Color color;

        ShaderVariable<Math3D::Matrix4x4>* modelCFrameVariable;
        ShaderVariable<Math3D::Matrix4x4>* modelRotationVariable;
        ShaderVariable<Math3D::Vector4>* modelSizeVariable;
        ShaderVariable<Color>* modelColorVariable;
	};
}

#endif
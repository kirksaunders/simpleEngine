#ifndef SHADERVARIABLE_HPP
#define SHADERVARIABLE_HPP

#define GLEW_STATIC

#include <vector>
#include <utility>
#include <string>

#include <GLEW/glew.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"

namespace Render3D {
    class Window;
    class Shader; // forward declaration

    class ShaderVariableInterface {
     public:
        virtual ~ShaderVariableInterface() {}

        virtual void setValue(const Window& win, const int& val) {}
        virtual void setValue(const Window& win, const float& val) {}
        virtual void setValue(const Window& win, const Math3D::Matrix4x4& val) {}
        virtual void setValue(const Window& win, const Math3D::Vector4& val) {}
        virtual void setValue(const Window& win, const Color& val) {}
    };

    template <class T>
    class ShaderVariable : public ShaderVariableInterface {
     public:
        ShaderVariable() : shader(nullptr) {}
        ShaderVariable(Shader* const s, const std::string& n) : shader(s), name(n) {}

        virtual void setValue(const Window& win, const T& val);
     private:
	 	typedef std::pair<GLuint, T> VariablePair;
        typedef std::vector<std::pair<GLuint, VariablePair> > VariableMap;

        Shader* shader;
        std::string name;
        VariableMap variables;
    };
};

#endif
#ifndef SHADERVARIABLE_HPP
#define SHADERVARIABLE_HPP

#include <string>
#include <utility>
#include <vector>

#include <glew.h>

#include "math/matrix4x4.hpp"
#include "math/vector4.hpp"

#include "render_base/color.hpp"

#ifndef ALWAYS_FALSE
#define ALWAYS_FALSE

namespace Render3D {
    template <class T>
    struct always_false { 
        static const bool value = false;
    };
};

#endif

namespace Render3D {
    // forward declarations
    class Shader;
    class Window;

    class ShaderVariableInterface {
     public:
        ShaderVariableInterface() : shader(nullptr), name("") {}
        ShaderVariableInterface(Shader& s, const std::string& n) : shader(&s), name(n) {}
        virtual ~ShaderVariableInterface() {}

        virtual void setValue(Window& win, const int& val) {}
        virtual void setValue(Window& win, const float& val) {}
        virtual void setValue(Window& win, const Math3D::Matrix4x4& val) {}
        virtual void setValue(Window& win, const Math3D::Vector4& val) {}
        virtual void setValue(Window& win, const Color& val) {}
        std::string getName() const { return name; }
     protected:
        Shader* shader;
        std::string name;
    };

    template <class T>
    class ShaderVariable : public ShaderVariableInterface {
     public:
        ShaderVariable() : ShaderVariableInterface() {}
        ShaderVariable(Shader& s, const std::string& n) : ShaderVariableInterface(s, n) {}

        template <class B>
        void setValue(Window& win, const B& val) { static_assert(always_false<B>::value, "Mismatched types when setting shader variable value"); }
        virtual void setValue(Window& win, const T& val);
     private:
        typedef std::pair<GLuint, T> VariablePair;
        typedef std::vector<std::pair<GLuint, VariablePair> > VariableMap;

        VariableMap variables;
    };
};

#endif
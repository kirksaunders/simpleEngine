#ifndef SHADERVARIABLEBLOCK_HPP
#define SHADERVARIABLEBLOCK_HPP

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "GL/glew.h"

#include "math/matrix4x4.hpp"
#include "math/vector4.hpp"

#include "objects/bufferobject.hpp"

#include "render_base/color.hpp"
#include "render_base/exception.hpp"
#include "render_base/uniformbuffermanager.hpp"
#include "render_base/window.hpp"

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
	// ShaderVariableBlock needs to be forward declared because ShaderBlockVariableInterface references it
	class ShaderVariableBlock;

    enum class GLSL_TYPE {
        BOOL,
        INT,
        UINT,
        FLOAT,
        VEC4,
        MAT4 // might do other matrices/vectors in the future, but this is fine for now
    };

    class ShaderBlockVariableInterface {
     public:
        ShaderBlockVariableInterface() : block(nullptr), offset(0), size(0), needsUpdated(false) {}
        ShaderBlockVariableInterface(ShaderVariableBlock& b, GLuint o, GLuint s) : block(&b), offset(o), size(s), needsUpdated(false) {}
        virtual ~ShaderBlockVariableInterface() {}

        virtual void setValue(Window& win, const bool& val) {}
        virtual void setValue(Window& win, const int& val) {}
        virtual void setValue(Window& win, const unsigned int& val) {}
        virtual void setValue(Window& win, const float& val) {}
        virtual void setValue(Window& win, const Math3D::Matrix4x4& val) {}
        virtual void setValue(Window& win, const Math3D::Vector4& val) {}
        virtual void setValue(Window& win, const Color& val) {}

        virtual void writeToArray(uint8_t* array) const;
        virtual void writeToBuffer(GLuint bufferID);
     private:
        virtual const void* getDataPointer() const = 0;
     protected:
        ShaderVariableBlock* block;
        GLuint offset;
        GLuint size;
        bool needsUpdated;
    };

    template <class T>
    class ShaderBlockVariable : public ShaderBlockVariableInterface {
     public:
        ShaderBlockVariable() : ShaderBlockVariableInterface(), value(T()) {}
        ShaderBlockVariable(ShaderVariableBlock& b, GLuint o, GLuint s) : ShaderBlockVariableInterface(b, o, s), value(T()) {}

        template <class B>
        void setValue(const B& val) { static_assert(always_false<B>::value, "Mismatched types when setting shader block variable value"); }
        virtual void setValue(const T& val);
     private:
        T value;

        virtual const void* getDataPointer() const;
    };

    template <class T>
    void ShaderBlockVariable<T>::setValue(const T& val) {
        if (val != value) {
            value = val;
            needsUpdated = true;
        }
    }

    class ShaderVariableBlock {
     public:
        ShaderVariableBlock() : size(0) {}
        ShaderVariableBlock(std::initializer_list<GLSL_TYPE> types);

        template <class T>
        ShaderBlockVariable<T>* getVariable(unsigned int index);

        int use(Window& win, UniformBufferManager& uniformBufferManager);

        void prepareContent(Window& win);
        void updateContent(Window& win);
        void destroyContent(Window& win);
     private:
        std::vector<std::unique_ptr<ShaderBlockVariableInterface> > variables;
        GLuint size;

        std::vector<std::pair<GLuint, BufferObject> > bufferObjects;

        int useNextLocation(GLuint bufferID, Window& win, UniformBufferManager& uniformBufferManager);

        void generateBuffer(GLuint clusterID);
        void destroyBuffer(GLuint clusterID);
        GLuint getBufferID(GLuint clusterID) const;
    };

    template <class T>
    ShaderBlockVariable<T>* ShaderVariableBlock::getVariable(unsigned int index) {
        if (index >= 0 && index < variables.size()) {
            return static_cast<ShaderBlockVariable<T>*>(variables[index].get());
        } else {
            throw Exception("Invalid index for variable of shader variable block");
        }
    }
};

#endif
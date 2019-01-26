#ifndef SHADER_HPP
#define SHADER_HPP

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "render_base/shadervariable.hpp"
#include "render_base/texturemanager.hpp"

namespace Render3D {
    // forward declarations
    class ShaderVariableBlock;
    class UniformBufferManager;
    class Window;

    struct ProgramID {
        GLuint id;
        GLuint useCount;

        ProgramID() : id(), useCount() {}
        ProgramID(GLuint i) : id(i), useCount(1) {}
    };

    class Shader {
     public:
        Shader() = delete;
        Shader(const Shader& other);
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        Shader& operator=(const Shader& other);

        void prepareContent(Window& win);
        void destroyContent(Window& win);
        void use(Window& win);
        void unuse(Window& win);

        template <class T>
        ShaderVariable<T>* getVariable(const std::string& name);

        ShaderVariable<int>* getDiffuseVariable(unsigned int num);
        ShaderVariable<int>* getSpecularVariable(unsigned int num);

        void bindVariableBlock(Window& win, UniformBufferManager& uniformBufferManager, const std::string& name, ShaderVariableBlock& block);

        GLuint getProgramID(GLuint clusterID);

        static Shader defaultPerspective();
        static Shader defaultImage();

     private:
        std::string vertexSource;
        std::string fragmentSource;
        std::vector<std::pair<GLuint, ProgramID> > programIDs;
        typedef std::vector<std::unique_ptr<ShaderVariableInterface> > VariablesMap;
        VariablesMap variables;
        std::array<ShaderVariable<int>*, TextureManager::MAX_MATERIAL_TEXTURES> diffTextureVariables;
        std::array<ShaderVariable<int>*, TextureManager::MAX_MATERIAL_TEXTURES> specTextureVariables;

        void compileProgram(GLuint clusterID);
        void destroyProgram(GLuint clusterID);
    };

    template <class T>
    ShaderVariable<T>* Shader::getVariable(const std::string& name) {
        for (unsigned int i = 0; i < variables.size(); ++i) {
            if (variables[i]->getName() == name) {
                return static_cast<ShaderVariable<T>*>(variables[i].get());
            }
        }

        std::unique_ptr<ShaderVariableInterface> ptr(new ShaderVariable<T>(*this, name));
        variables.push_back(std::move(ptr));
        return static_cast<ShaderVariable<T>*>(variables.back().get());
    }
}

#endif
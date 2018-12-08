#ifndef SHADER_HPP
#define SHADER_HPP

#define GLEW_STATIC

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>

#include <GLEW/glew.h>

#include "render_base/shadervariable.hpp"
#include "render_base/texturemanager.hpp"

namespace Render3D {
    class Window; // forward declaration

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

        void prepareContent(const Window& win);
        void destroyContent(const Window& win);
		void use(const Window& win);

        template <class T>
		ShaderVariable<T>* getVariable(const std::string& name);

		ShaderVariable<int>* getDiffuseVariable(unsigned int num);
		ShaderVariable<int>* getSpecularVariable(unsigned int num);

		GLuint getProgramID(GLuint clusterID);

        static Shader defaultPerspective();
        static Shader defaultImage();

	 private:
        std::string vertexSource;
        std::string fragmentSource;
        std::vector<std::pair<GLuint, ProgramID> > programIDs;
        typedef std::unordered_map<std::string, std::unique_ptr<ShaderVariableInterface> > VariablesMap;
        VariablesMap variables;
		std::array<ShaderVariable<int>*, TextureManager::MAX_MATERIAL_TEXTURES> diffTextureVariables;
		std::array<ShaderVariable<int>*, TextureManager::MAX_MATERIAL_TEXTURES> specTextureVariables;

        void compileProgram(GLuint clusterID);
        void destroyProgram(GLuint clusterID);
	};

    template <class T>
    ShaderVariable<T>* Shader::getVariable(const std::string& name) {
        VariablesMap::iterator it = variables.find(name);
        if (it != variables.end()) {
            return static_cast<ShaderVariable<T>*>(it->second.get());
        } else {
            std::unique_ptr<ShaderVariableInterface> ptr(new ShaderVariable<T>(this, name));
            std::pair<VariablesMap::iterator, bool> result = variables.insert(std::pair<std::string, std::unique_ptr<ShaderVariableInterface> >(name, std::move(ptr)));
            return static_cast<ShaderVariable<T>*>(result.first->second.get());
        }
    }
}

#endif
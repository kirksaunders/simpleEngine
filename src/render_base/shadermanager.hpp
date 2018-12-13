#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <string>
#include <unordered_map>

#include "render_base/shader.hpp"

namespace Render3D {
    class ShaderManager {
     public:
        bool shaderDoesExist(const std::string& shaderName) const;

        Shader* getShader(const std::string& shaderName);

        void addShader(const std::string& shaderName, const Shader& shader);
        
     private:
        std::unordered_map<std::string, Shader> shaders;
    };
}

#endif
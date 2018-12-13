#include "render_base/shadermanager.hpp"

using namespace Render3D;

bool ShaderManager::shaderDoesExist(const std::string& shaderName) const {
    return shaders.count(shaderName) == 1;
}

Shader* ShaderManager::getShader(const std::string& shaderName) {
    return &shaders.at(shaderName);
}

void ShaderManager::addShader(const std::string& shaderName, const Shader& shader) {
    shaders.insert(std::pair<std::string, Shader>(shaderName, shader));
}
#include "render_base/shadermanager.hpp"

using namespace Render3D;

bool ShaderManager::shaderDoesExist(char const *shaderName) const {
	return shaders.count(std::string(shaderName)) == 1;
}

Shader* ShaderManager::getShader(char const *shaderName) {
	return &shaders.at(std::string(shaderName));
}

void ShaderManager::addShader(char const *shaderName, const Shader& shader) {
	shaders.insert(std::pair<std::string, Shader>(std::string(shaderName), shader));
}

namespace Render3D {
    ShaderManager shaderManager;
}

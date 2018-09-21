#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <string>
#include <unordered_map>

#include "render_base/shader.hpp"

namespace Render3D {
	class ShaderManager {
	 public:
		bool shaderDoesExist(char const *shaderName) const;

		Shader* getShader(char const *shaderName);

		void addShader(char const *shaderName, const Shader& shader);
		
	 private:
		std::unordered_map<std::string, Shader> shaders;
	};

	extern ShaderManager shaderManager;
}

#endif
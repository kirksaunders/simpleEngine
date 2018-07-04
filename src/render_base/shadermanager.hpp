#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <map>
#include <string>

#include "render_base/shader.hpp"

namespace Render3D {
	class ShaderManager {
	 public:
		bool shaderDoesExist(char const *shaderName) const;

		Shader* getShader(char const *shaderName);

		void addShader(char const *shaderName, const Shader& shader);
		
	 private:
		std::map<std::string, Shader> shaders;
	};

	extern ShaderManager shaderManager;
}

#endif
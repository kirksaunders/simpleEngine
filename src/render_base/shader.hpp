#ifndef SHADER_HPP
#define SHADER_HPP

#define GLEW_STATIC

#include <string>
#include <unordered_map>

#include <GLEW/glew.h>

#include "math/vector4.hpp"
#include "math/matrix4x4.hpp"

#include "render_base/color.hpp"

namespace Render3D {
	class Shader {
	 public:
		Shader() {}

		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

		void use();

		void setVariable(char const *variableName, int number);

		void setVariable(char const *variableName, float number);

		void setVariable(char const *variableName, const Math3D::Matrix4x4& matrix);

		void setVariable(char const *variableName, const Math3D::Vector4& vector);

		void setVariable(char const *variableName, const Color& color);

		GLuint getProgramID();

	 private:
		GLuint programID;

        std::unordered_map<std::string, std::pair<GLuint, int> > integers;

        std::unordered_map<std::string, std::pair<GLuint, float> > floats;

        std::unordered_map<std::string, std::pair<GLuint, Math3D::Matrix4x4> > matrices;

        std::unordered_map<std::string, std::pair<GLuint, Math3D::Vector4> > vectors;

        std::unordered_map<std::string, std::pair<GLuint, Color> > colors;
	};
}

#endif
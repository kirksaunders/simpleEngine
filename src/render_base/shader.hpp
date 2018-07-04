#ifndef SHADER_HPP
#define SHADER_HPP

#define GLEW_STATIC

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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

		void setVariable(char const *variableName, Math3D::Matrix4x4& matrix);

		void setVariable(char const *variableName, Math3D::Vector4& vector);

		void setVariable(char const *variableName, Color& color);

		GLuint getProgramID();

	 private:
		GLuint programID;

		std::vector<std::string> integerIndices;
		std::vector<GLuint> integerLocations;
		std::vector<int> integerValues;

		std::vector<std::string> floatIndices;
		std::vector<GLuint> floatLocations;
		std::vector<float> floatValues;

		std::vector<std::string> matrixIndices;
		std::vector<GLuint> matrixLocations;
		std::vector<Math3D::Matrix4x4> matrixValues;

		std::vector<std::string> vectorIndices;
		std::vector<GLuint> vectorLocations;
		std::vector<Math3D::Vector4> vectorValues;

		std::vector<std::string> colorIndices;
		std::vector<GLuint> colorLocations;
		std::vector<Color> colorValues;
	};
}

#endif
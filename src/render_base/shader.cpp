#include "render_base/shader.hpp"

using namespace Render3D;
using namespace Math3D;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into GLchar array
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();		
	} catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();	
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	   
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	  
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	  
	// Shader Program
	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	// Print linking errors if any
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	  
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(programID);
}

void Shader::setVariable(char const *variableName, int number) {
	unsigned int index;
	GLuint variableLocation;
	std::string name(variableName);
	unsigned int size = integerIndices.size();
	for (index = 0; index < size; index++) {
		if (integerIndices[index] == name) {
			break;
		}
	}
	if (index < size) {
		if (integerValues[index] == number) {
			return;
		}
		variableLocation = integerLocations[index];
		integerValues[index] = number;
	} else {
		variableLocation = glGetUniformLocation(programID, variableName);
		integerIndices.push_back(name);
		integerLocations.push_back(variableLocation);
		integerValues.push_back(number);
	}
	glUniform1i(variableLocation, number);
}

void Shader::setVariable(char const *variableName, float number) {
	unsigned int index;
	GLuint variableLocation;
	std::string name(variableName);
	unsigned int size = floatIndices.size();
	for (index = 0; index < size; index++) {
		if (floatIndices[index] == name) {
			break;
		}
	}
	if (index < size) {
		if (floatValues[index] == number) {
			return;
		}
		variableLocation = floatLocations[index];
		floatValues[index] = number;
	} else {
		variableLocation = glGetUniformLocation(programID, variableName);
		floatIndices.push_back(name);
		floatLocations.push_back(variableLocation);
		floatValues.push_back(number);
	}
	glUniform1f(variableLocation, number);
}

void Shader::setVariable(char const *variableName, Matrix4x4& matrix) {
	unsigned int index;
	GLuint variableLocation;
	std::string name(variableName);
	unsigned int size = matrixIndices.size();
	for (index = 0; index < size; index++) {
		if (matrixIndices[index] == name) {
			break;
		}
	}
	if (index < size) {
		if (matrixValues[index] == matrix) {
			return;
		}
		variableLocation = matrixLocations[index];
		matrixValues[index] = matrix;
	} else {
		variableLocation = glGetUniformLocation(programID, variableName);
		matrixIndices.push_back(name);
		matrixLocations.push_back(variableLocation);
		matrixValues.push_back(matrix);
	}
	glUniformMatrix4fv(variableLocation, 1, GL_TRUE, matrix.getValues());
}

void Shader::setVariable(char const *variableName, Vector4& vector) {
	unsigned int index;
	GLuint variableLocation;
	std::string name(variableName);
	unsigned int size = vectorIndices.size();
	for (index = 0; index < size; index++) {
		if (vectorIndices[index] == name) {
			break;
		}
	}
	if (index < size) {
		if (vectorValues[index] == vector) {
			return;
		}
		variableLocation = vectorLocations[index];
		vectorValues[index] = vector;
	} else {
		variableLocation = glGetUniformLocation(programID, variableName);
		vectorIndices.push_back(name);
		vectorLocations.push_back(variableLocation);
		vectorValues.push_back(vector);
	}
	//glUniform4f(variableLocation, vector[0], vector[1], vector[2], vector[3]);
	glUniform4fv(variableLocation, 1, vector.getValues());
}

void Shader::setVariable(char const *variableName, Color& color) {
	unsigned int index;
	GLuint variableLocation;
	std::string name(variableName);
	unsigned int size = colorIndices.size();
	for (index = 0; index < size; index++) {
		if (colorIndices[index] == name) {
			break;
		}
	}
	if (index < size) {
		if (colorValues[index] == color) {
			return;
		}
		variableLocation = colorLocations[index];
		colorValues[index] = color;
	} else {
		variableLocation = glGetUniformLocation(programID, variableName);
		colorIndices.push_back(name);
		colorLocations.push_back(variableLocation);
		colorValues.push_back(color);
	}
	glUniform4fv(variableLocation, 1, color.getValues());
}

GLuint Shader::getProgramID() {
	return programID;
}

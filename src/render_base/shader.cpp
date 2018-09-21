#include <cmath>
#include <fstream>
#include <sstream>

#include "math/constants.hpp"

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
    std::string name(variableName);
    std::unordered_map<std::string, std::pair<GLuint, int> >::iterator it;
    it = integers.find(name);
    GLuint variableLocation;
    if (it == integers.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, int> values(variableLocation, number);
        integers.insert(std::pair<std::string, std::pair<GLuint, int> >(name, values));
    } else {
        if (it->second.second == number) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = number;
    }
    glUniform1i(variableLocation, number);
}

void Shader::setVariable(char const *variableName, float number) {
    std::string name(variableName);
    std::unordered_map<std::string, std::pair<GLuint, float> >::iterator it;
    it = floats.find(name);
    GLuint variableLocation;
    if (it == floats.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, float> values(variableLocation, number);
        floats.insert(std::pair<std::string, std::pair<GLuint, float> >(name, values));
    } else {
        if (it->second.second == number) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = number;
    }
    glUniform1f(variableLocation, number);
}

void Shader::setVariable(char const *variableName, const Matrix4x4& matrix) {
    std::string name(variableName);
    std::unordered_map<std::string, std::pair<GLuint, Matrix4x4> >::iterator it;
    it = matrices.find(name);
    GLuint variableLocation;
    if (it == matrices.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Matrix4x4> values(variableLocation, matrix);
        matrices.insert(std::pair<std::string, std::pair<GLuint, Matrix4x4> >(name, values));
    } else {
        if (it->second.second == matrix) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = matrix;
    }
	glUniformMatrix4fv(variableLocation, 1, GL_TRUE, matrix.getValues());
}

void Shader::setVariable(char const *variableName, const Vector4& vector) {
    std::string name(variableName);
    std::unordered_map<std::string, std::pair<GLuint, Vector4> >::iterator it;
    it = vectors.find(name);
    GLuint variableLocation;
    if (it == vectors.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Vector4> values(variableLocation, vector);
        vectors.insert(std::pair<std::string, std::pair<GLuint, Vector4> >(name, values));
    } else {
        if (it->second.second == vector) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = vector;
    }
	glUniform4fv(variableLocation, 1, vector.getValues());
}

void Shader::setVariable(char const *variableName, const Color& color) {
    std::string name(variableName);
    std::unordered_map<std::string, std::pair<GLuint, Color> >::iterator it;
    it = colors.find(name);
    GLuint variableLocation;
    if (it == colors.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Color> values(variableLocation, color);
        colors.insert(std::pair<std::string, std::pair<GLuint, Color> >(name, values));
    } else {
        if (it->second.second == color) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = color;
    }
	glUniform4fv(variableLocation, 1, color.getValues());
}

GLuint Shader::getProgramID() {
	return programID;
}

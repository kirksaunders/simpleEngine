#include <cmath>
#include <fstream>
#include <sstream>

#include "render_base/shader.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the vertex/fragment source code from filePath
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
		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();		
	} catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

void Shader::prepareContent(Window* win) {
    win->makeCurrent();
    compileProgram(win->getClusterID());
}

void Shader::use(Window* win) {
	glUseProgram(getProgramID(win->getClusterID()));
}

GLuint Shader::compileProgram(GLuint clusterID) {
    std::unordered_map<GLuint, GLuint>::iterator it = programIDs.find(clusterID);
    if (it != programIDs.end()) {
        glUseProgram(it->second);
        glUseProgram(0);
        return it->second;
    }

    GLuint programID;

	const GLchar* vShaderCode = vertexSource.c_str();	
	const GLchar* fShaderCode = fragmentSource.c_str();

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

    // Now that the shaders have been linked to the shader program, we can detach them
    glDetachShader(programID, vertex);
    glDetachShader(programID, fragment);
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

    programIDs.insert(std::pair<GLuint, GLuint>(clusterID, programID));

    return programID;
}

GLuint Shader::getProgramID(GLuint clusterID) {
    std::unordered_map<GLuint, GLuint>::iterator it = programIDs.find(clusterID);
    if (it != programIDs.end()) {
        return it->second;
    } else {
        return compileProgram(clusterID);
    }
}

void Shader::setVariable(Window* win, const char *variableName, int number) {
    GLuint clusterID = win->getClusterID();
    GLuint programID = getProgramID(clusterID);
    std::string name(variableName);
    IntegerMap::iterator it = integers.find(KeyPair(name, clusterID));
    GLuint variableLocation;
    if (it == integers.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, int> values(variableLocation, number);
        integers.insert(std::pair<KeyPair, std::pair<GLuint, int> >(KeyPair(name, clusterID), values));
    } else {
        if (it->second.second == number) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = number;
    }
    glUniform1i(variableLocation, number);
}

void Shader::setVariable(Window* win, const char *variableName, float number) {
    GLuint clusterID = win->getClusterID();
    GLuint programID = getProgramID(clusterID);
    std::string name(variableName);
    FloatMap::iterator it = floats.find(KeyPair(name, clusterID));
    GLuint variableLocation;
    if (it == floats.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, float> values(variableLocation, number);
        floats.insert(std::pair<KeyPair, std::pair<GLuint, float> >(KeyPair(name, clusterID), values));
    } else {
        if (it->second.second == number) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = number;
    }
    glUniform1f(variableLocation, number);
}

void Shader::setVariable(Window* win, const char *variableName, const Matrix4x4& matrix) {
    GLuint clusterID = win->getClusterID();
    GLuint programID = getProgramID(clusterID);
    std::string name(variableName);
    MatrixMap::iterator it = matrices.find(KeyPair(name, clusterID));
    GLuint variableLocation;
    if (it == matrices.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Matrix4x4> values(variableLocation, matrix);
        matrices.insert(std::pair<KeyPair, std::pair<GLuint, Matrix4x4> >(KeyPair(name, clusterID), values));
    } else {
        if (it->second.second == matrix) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = matrix;
    }
	glUniformMatrix4fv(variableLocation, 1, GL_TRUE, matrix.getValues());
}

void Shader::setVariable(Window* win, const char *variableName, const Vector4& vector) {
    GLuint clusterID = win->getClusterID();
    GLuint programID = getProgramID(clusterID);
    std::string name(variableName);
    VectorMap::iterator it = vectors.find(KeyPair(name, clusterID));
    GLuint variableLocation;
    if (it == vectors.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Vector4> values(variableLocation, vector);
        vectors.insert(std::pair<KeyPair, std::pair<GLuint, Vector4> >(KeyPair(name, clusterID), values));
    } else {
        if (it->second.second == vector) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = vector;
    }
	glUniform4fv(variableLocation, 1, vector.getValues());
}

void Shader::setVariable(Window* win, const char *variableName, const Color& color) {
    GLuint clusterID = win->getClusterID();
    GLuint programID = getProgramID(clusterID);
    std::string name(variableName);
    ColorMap::iterator it = colors.find(KeyPair(name, clusterID));
    GLuint variableLocation;
    if (it == colors.end()) {
        variableLocation = glGetUniformLocation(programID, variableName);
        std::pair<GLuint, Color> values(variableLocation, color);
        colors.insert(std::pair<KeyPair, std::pair<GLuint, Color> >(KeyPair(name, clusterID), values));
    } else {
        if (it->second.second == color) {
            return;
        }
        variableLocation = it->second.first;
        it->second.second = color;
    }
	glUniform4fv(variableLocation, 1, color.getValues());
}

#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "render_base/exception.hpp"
#include "render_base/shader.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

Shader::Shader(const Shader& other) : vertexSource(other.vertexSource), fragmentSource(other.fragmentSource) {
    for (unsigned int i = 0; i < TextureManager::MAX_MATERIAL_TEXTURES; i++) {
        diffTextureVariables[i] = getVariable<int>(TextureManager::getDiffuseName(i));
        specTextureVariables[i] = getVariable<int>(TextureManager::getSpecularName(i));
    }
}

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
        throw Exception("Shader file loading failed");
    }

    for (unsigned int i = 0; i < TextureManager::MAX_MATERIAL_TEXTURES; i++) {
        diffTextureVariables[i] = getVariable<int>(TextureManager::getDiffuseName(i));
        specTextureVariables[i] = getVariable<int>(TextureManager::getSpecularName(i));
    }
}

Shader& Shader::operator=(const Shader& other) {
    if (this != &other) {
        vertexSource = other.vertexSource;
        fragmentSource = other.fragmentSource;
        programIDs.clear();
        variables.clear();

        for (unsigned int i = 0; i < TextureManager::MAX_MATERIAL_TEXTURES; i++) {
            diffTextureVariables[i] = getVariable<int>(TextureManager::getDiffuseName(i));
            specTextureVariables[i] = getVariable<int>(TextureManager::getSpecularName(i));
        }
    }

    return *this;
}

void Shader::prepareContent(Window& win) {
    compileProgram(win.getClusterID());
}

void Shader::destroyContent(Window& win) {
    destroyProgram(win.getClusterID());
}

void Shader::use(Window& win) {
    if (!win.isShaderActive(*this)) {
        win.setShaderActive(*this);
        glUseProgram(getProgramID(win.getClusterID()));
    }
}

void Shader::unuse(Window& win) {
    if (win.isShaderActive(*this)) {
        win.setShaderActive(*this, false);
        glUseProgram(0);
    }
}

void Shader::compileProgram(GLuint clusterID) {
    for (unsigned int i = 0; i < programIDs.size(); ++i) {
        if (programIDs[i].first == clusterID) {
            glUseProgram(programIDs[i].second.id);
            glUseProgram(0);
            ++(programIDs[i].second.useCount);

            return;
        }
    }

    ProgramID programID;

    const GLchar* vShaderCode = vertexSource.c_str();    
    const GLchar* fShaderCode = fragmentSource.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
       
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        throw Exception(std::string("Shader vertex code compilation failed: ") + infoLog);
    }
      
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        throw Exception(std::string("Shader fragment code compilation failed: ") + infoLog);
    }
      
    // Shader Program
    programID.id = glCreateProgram();
    glAttachShader(programID.id, vertex);
    glAttachShader(programID.id, fragment);
    glLinkProgram(programID.id);

    // Print linking errors if any
    glGetProgramiv(programID.id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID.id, 512, nullptr, infoLog);
        throw Exception(std::string("Shader linking failed: ") + infoLog);
    }

    // Now that the shaders have been linked to the shader program, we can detach them
    glDetachShader(programID.id, vertex);
    glDetachShader(programID.id, fragment);
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    programID.useCount = 1;
    programIDs.push_back(std::pair<GLuint, ProgramID>(clusterID, std::move(programID)));
}

void Shader::destroyProgram(GLuint clusterID) {
    for (unsigned int i = 0; i < programIDs.size(); ++i) {
        if (programIDs[i].first == clusterID) {
            if (--(programIDs[i].second.useCount) == 0) {
                glDeleteShader(programIDs[i].second.id);
                std::swap(programIDs[i], programIDs.back());
                programIDs.pop_back();
            }
            return;
        }
    }
}

GLuint Shader::getProgramID(GLuint clusterID) {
    for (unsigned int i = 0; i < programIDs.size(); ++i) {
        if (programIDs[i].first == clusterID) {
            return programIDs[i].second.id;
        }
    }

    throw Exception("Unable to use shader, it hasn't been added to the context");
}

ShaderVariable<int>* Shader::getDiffuseVariable(unsigned int num) {
    return diffTextureVariables[num];
}

ShaderVariable<int>* Shader::getSpecularVariable(unsigned int num) {
    return specTextureVariables[num];
}

Shader Shader::defaultPerspective() {
    return Shader("res/defaultPerspective.vert", "res/defaultPerspective.frag");
}

Shader Shader::defaultImage() {
    return Shader("res/imageRender.vert", "res/imageRender.frag");
}

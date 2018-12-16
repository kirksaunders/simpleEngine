#include <algorithm>
#include <utility>
#include <iostream>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "render_base/exception.hpp"
#include "render_base/window.hpp"
#include "render_base/texture.hpp"
#include "render_base/shader.hpp"
#include "render_base/texturemanager.hpp"
#include "render_base/texturebuffer.hpp"

using namespace Render3D;

Texture::Texture(int w, int h) {
    width = w;
    height = h;
    depth = 4;
    textureName = "tex";
    shader = nullptr;
    image = nullptr;
    textureVariable = nullptr;
}

Texture::Texture(const char *filePath) {
    textureName = "tex";
    shader = nullptr;
    textureVariable = nullptr;

    image = stbi_load(filePath, &width, &height, &depth, 0);
	if (image == nullptr) {
		throw Exception(std::string("Unable to load texture from file ") + filePath);
	}
	if (depth != 3 && depth != 4) {
        throw Exception("Unable to load texture from file, the number of image channels must be either 3 or 4");
    }
    std::cout << filePath << std::endl;
    std::cout << width << " x " << height << std::endl;
}

Texture::Texture(const TextureBuffer& buff) {
    textureName = "tex";
    shader = nullptr;
    textureVariable = nullptr;

    width = buff.getWidth();
    height = buff.getHeight();
    depth = buff.getDepth();

    if (depth != 3 && depth != 4) {
        throw Exception("Unable to create texture from texture buffer, the depth of the texture buffer must be either 3 or 4");
    }

	image = static_cast<GLubyte*>(malloc(width * height * depth)); // to be consistent with stb_image, we use malloc and memcpy for the image data
	if (image == nullptr) {
		throw Exception("Unable to allocate memory for texture");
	}
	memcpy(image, buff.getAddress(), width * height * depth);
}

Texture::Texture(const Texture& other) {
    copy(other);
}

Texture::Texture(Texture&& other) noexcept {
    move(other);
    other.initialize();
}

Texture::~Texture() {
    destroy();
}

Texture& Texture::operator=(const Texture& other) {
    if (this != &other) {
        destroy();
        copy(other);
    }

    return *this;
}

Texture& Texture::operator=(Texture&& other) {
    if (this != &other) {
        destroy();
        move(other);
        other.initialize();
    }

    return *this;
}

/*void Texture::loadFromFile(const char *filePath) {
    int w, h;
    unsigned char* image = SOIL_load_image(filePath, &w, &h, 0, SOIL_LOAD_RGBA);
    std::cout << filePath << std::endl;
    std::cout << SOIL_last_result() << std::endl;
    std::cout << w << " x " << h << std::endl;

    glBindTexture(GL_TEXTURE_2D, textureID);
    if (w == width && h == height) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } else {
        width = w;
        height = h;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
}

void Texture::loadFromArray(int w, int h, GLubyte* data) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    if (w == width && h == height) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        width = w;
        height = h;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}*/

int Texture::getWidth() {
    return width;
}

int Texture::getHeight() {
    return height;
}

void Texture::setName(const std::string& name) {
    textureName = name;
}

const char* Texture::getName() {
    return textureName.c_str();
}

void Texture::setShader(Shader* s) {
    shader = s;

    if (shader != nullptr) {
        textureVariable = shader->getVariable<int>(textureName);
    } else {
        textureVariable = nullptr;
    }
}

Shader* const Texture::getShader() {
    return shader;
}

void Texture::use(Window& win, TextureManager& textureManager) {
    GLuint clusterID = win.getClusterID();
    GLuint textureID = getTextureID(clusterID);

    int location = textureManager.getTextureLocation(textureID, win);
    if (location == -1) {
        location = useNextLocation(textureID, win, textureManager);
    }
    textureVariable->setValue(win, location);
}

void Texture::useDiffuse(Shader& shader, Window& win, TextureManager& textureManager, unsigned int num) {
    GLuint clusterID = win.getClusterID();
    GLuint textureID = getTextureID(clusterID);

    int location = textureManager.getTextureLocation(textureID, win);
    if (location == -1) {
        location = useNextLocation(textureID, win, textureManager);
    }
    
    shader.getDiffuseVariable(num)->setValue(win, location);
}

void Texture::useSpecular(Shader& shader, Window& win, TextureManager& textureManager, unsigned int num) {
    GLuint clusterID = win.getClusterID();
    GLuint textureID = getTextureID(clusterID);

    int location = textureManager.getTextureLocation(textureID, win);
    if (location == -1) {
        location = useNextLocation(textureID, win, textureManager);
    }

    shader.getSpecularVariable(num)->setValue(win, location);
}

void Texture::resetDiffAndSpec(Shader& shader, Window& win, TextureManager& textureManager) {
    GLuint clusterID = win.getClusterID();
    GLuint textureID = getTextureID(clusterID);

    int location = textureManager.getTextureLocation(textureID, win);
    if (location == -1) {
        location = useNextLocation(textureID, win, textureManager);
    }

    for (unsigned int i = 0; i < TextureManager::MAX_MATERIAL_TEXTURES; i++) {
        shader.getDiffuseVariable(i)->setValue(win, location);
        shader.getSpecularVariable(i)->setValue(win, location);
    }
}

int Texture::useNextLocation(GLuint textureID, Window& win, TextureManager& textureManager) {
    int location = textureManager.makeTextureActive(textureID, win);
    glActiveTexture(GL_TEXTURE0 + location);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0);

    return location;
}

BufferPair Texture::generateBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            ++(bufferObjects[i].second.useCount);
            return bufferObjects[i].second;
        }
    }

    GLfloat vertices[12];
    vertices[0] = 1.0;
    vertices[1] = 1.0;
    
    vertices[2] = -1.0;
    vertices[3] = 1.0;

    vertices[4] = -1.0;
    vertices[5] = -1.0;

    vertices[6] = 1.0;
    vertices[7] = 1.0;

    vertices[8] = -1.0;
    vertices[9] = -1.0;

    vertices[10] = 1.0;
    vertices[11] = -1.0;

    GLfloat texCoords[12];
    texCoords[0] = 1.0;
    texCoords[1] = 0.0;

    texCoords[2] = 0.0;
    texCoords[3] = 0.0;

    texCoords[4] = 0.0;
    texCoords[5] = 1.0;

    texCoords[6] = 1.0;
    texCoords[7] = 0.0;

    texCoords[8] = 0.0;
    texCoords[9] = 1.0;

    texCoords[10] = 1.0;
    texCoords[11] = 1.0;

    BufferPair pair;

    glGenBuffers(1, &pair.first);
    glBindBuffer(GL_ARRAY_BUFFER, pair.first);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &pair.second);
    glBindBuffer(GL_ARRAY_BUFFER, pair.second);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    pair.useCount = 1;
    bufferObjects.push_back(std::pair<GLuint, BufferPair>(clusterID, pair));

    return pair;
}

void Texture::destroyBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            if (--(bufferObjects[i].second.useCount) == 0) {
                glDeleteBuffers(1, &bufferObjects[i].second.first);
                glDeleteBuffers(1, &bufferObjects[i].second.second);
                std::swap(bufferObjects[i], bufferObjects.back());
                bufferObjects.pop_back();
            }
            return;
        }
    }
}

void Texture::generateTexture(GLuint clusterID) {
    for (unsigned int i = 0; i < IDs.size(); ++i) {
        if (IDs[i].first == clusterID) {
            ++(IDs[i].second.useCount);
            
            return;
        }
    }

    TextureID textureID;

    glGenTextures(1, &textureID.id);
    glBindTexture(GL_TEXTURE_2D, textureID.id);
    if (depth == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    textureID.useCount = 1;
    IDs.push_back(std::pair<GLuint, TextureID>(clusterID, std::move(textureID)));
}

void Texture::destroyTexture(GLuint clusterID) {
    for (unsigned int i = 0; i < IDs.size(); ++i) {
        if (IDs[i].first == clusterID) {
            if (--(IDs[i].second.useCount) == 0) {
                glDeleteTextures(1, &IDs[i].second.id);
                std::swap(IDs[i], IDs.back());
                IDs.pop_back();
            }
            return;
        }
    }
}

void Texture::generateVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return;
        }
    }

    BufferPair pair = generateBuffers(win.getClusterID());

    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, pair.first);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pair.second);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    VAOs.push_back(std::pair<const Window*, GLuint>(&win, VAO));
}

void Texture::destroyVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            glDeleteVertexArrays(1, &VAOs[i].second);
            std::swap(VAOs[i], VAOs.back());
            VAOs.pop_back();
            return;
        }
    }
}

GLuint Texture::getTextureID(GLuint clusterID) {
    for (unsigned int i = 0; i < IDs.size(); ++i) {
        if (IDs[i].first == clusterID) {
            return IDs[i].second.id;
        }
    }
    
    return 0;
}

GLuint Texture::getVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return VAOs[i].second;
        }
    }
    
    return 0;
}

void Texture::render(Window& win, TextureManager& textureManager) {
    use(win, textureManager);

    glBindVertexArray(getVertexArrayObject(win));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Texture::prepareContent(Window& win, TextureManager& textureManager) {
    generateTexture(win.getClusterID());
    generateVertexArrayObject(win);
}

void Texture::destroyContent(Window& win, TextureManager& textureManager) {
    GLuint clusterID = win.getClusterID();
    textureManager.makeTextureInactive(getTextureID(clusterID), win);
    destroyVertexArrayObject(win);
    destroyBuffers(clusterID);
    destroyTexture(clusterID);
}

void Texture::copy(const Texture& other) {
    width = other.width;
    height = other.height;
    depth = other.depth;
    textureName = other.textureName;
    shader = other.shader;
    textureVariable = other.textureVariable;
    bufferObjects = other.bufferObjects;
    VAOs = other.VAOs;
    IDs = other.IDs;
    if (other.image != nullptr) {
        image = static_cast<GLubyte*>(malloc(width * height * depth)); // to be consistent with stb_image, we use malloc and memcpy for the image data
		if (image == nullptr) {
			throw Exception("Unable to allocate memory for texture");
		}
		memcpy(image, other.image, width * height * depth);
    } else {
        image = nullptr;
    }
}

void Texture::move(Texture& other) {
    width = other.width;
    height = other.height;
    depth = other.depth;
    shader = other.shader;
    textureVariable = other.textureVariable;
    textureName = std::move(other.textureName);
    bufferObjects = std::move(other.bufferObjects);
    VAOs = std::move(other.VAOs);
    IDs = std::move(other.IDs);
    image = other.image;
}

void Texture::initialize() {
    width = 0;
    height = 0;
    depth = 4;
    textureName = "tex";
    shader = nullptr;
    textureVariable = nullptr;
    bufferObjects.clear();
    VAOs.clear();
    IDs.clear();
    image = nullptr;
}

void Texture::destroy() {
    if (image != nullptr) {
        free(image); // stb_image uses malloc to allocate image, so to deallocate we should use free
    }
}

#include <algorithm>
#include <utility>

#include <SOIL/SOIL.h>

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
	shaderName = "imageRender";
    image = NULL;
}

Texture::Texture(const char *filePath) {
	textureName = "tex";
	shaderName = "imageRender";

	image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);
    depth = 4;
	std::cout << filePath << std::endl;
	std::cout << SOIL_last_result() << std::endl;
	std::cout << width << " x " << height << std::endl;
}

Texture::Texture(const TextureBuffer& buff) {
    textureName = "tex";
    shaderName = "imageRender";

    width = buff.getWidth();
    height = buff.getHeight();
    depth = buff.getDepth();

    if (depth != 3 && depth != 4) {
        // error
        return;
    }

    image = new GLubyte[width * height * depth];
    std::copy(buff.getAddress(), buff.getAddress() + (width * height * depth), image);
}

Texture::Texture(const Texture& other) {
    copy(other);
}

Texture::Texture(Texture&& other) {
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

void Texture::setShader(const std::string& name) {
    shaderName = name;
}

const char* Texture::getShader() {
	return shaderName.c_str();
}

void Texture::use(Shader* shader, Window* win, TextureManager* textureManager) {
    GLuint clusterID = win->getClusterID();
    GLuint textureID = getTextureID(clusterID);

	int location = getLocation(textureID, win, textureManager);
	if (location == -1) {
		location = useNextLocation(textureID, win, textureManager);
	}
	shader->setVariable(win, textureName.c_str(), location);
}

void Texture::use(Shader* shader, Window* win, TextureManager* textureManager, const char* name) {
    GLuint clusterID = win->getClusterID();
    GLuint textureID = getTextureID(clusterID);

	int location = getLocation(textureID, win, textureManager);
	if (location == -1) {
		location = useNextLocation(textureID, win, textureManager);
	}
	shader->setVariable(win, name, location);
}

int Texture::getLocation(GLuint textureID, Window* win, TextureManager* textureManager) {
	for (int i = 0; i < textureManager->getNumberTextures(win); i++) {
		if (textureManager->getActiveTexture(i, win) == textureID) {
			return i;
		}
	}

	return -1;
}

int Texture::useNextLocation(GLuint textureID, Window* win, TextureManager* textureManager) {
	int location = textureManager->getNumberTextures(win);
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, textureID);
	textureManager->setActive(textureID, location, win);
	glActiveTexture(GL_TEXTURE0);
	return location;
}

void Texture::generateBuffers(GLuint clusterID) {
    std::unordered_map<GLuint, BufferPair>::iterator it = bufferObjects.find(clusterID);
    if (it != bufferObjects.end()) {
        return;
    }

	GLfloat vertices[12];
	vertices[0] = -1.0;
	vertices[1] = 1.0;
	
	vertices[2] = 1.0;
	vertices[3] = 1.0;

	vertices[4] = -1.0;
	vertices[5] = -1.0;

	vertices[6] = 1.0;
	vertices[7] = 1.0;

	vertices[8] = 1.0;
	vertices[9] = -1.0;

	vertices[10] = -1.0;
	vertices[11] = -1.0;

	GLfloat texCoords[12];
	texCoords[0] = 0.0;
	texCoords[1] = 0.0;

	texCoords[2] = 1.0;
	texCoords[3] = 0.0;

	texCoords[4] = 0.0;
	texCoords[5] = 1.0;

	texCoords[6] = 1.0;
	texCoords[7] = 0.0;

	texCoords[8] = 1.0;
	texCoords[9] = 1.0;

	texCoords[10] = 0.0;
	texCoords[11] = 1.0;

    GLuint VBO, TBO;

    glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

    bufferObjects.insert(std::pair<GLuint, BufferPair>(clusterID, BufferPair(VBO, TBO)));
}

void Texture::generateTexture(GLuint clusterID) {
    std::unordered_map<GLuint, GLuint>::iterator it = IDs.find(clusterID);
    if (it != IDs.end()) {
        return;
    }

    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
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

    IDs.insert(std::pair<GLuint, GLuint>(clusterID, textureID));
}

void Texture::generateVertexArrayObject(GLuint clusterID, Window* win) {
    std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
    if (it != VAOs.end()) {
        return;
    }

    std::unordered_map<GLuint, BufferPair>::iterator it2 = bufferObjects.find(clusterID);

    GLuint VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, it2->second.first);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, it2->second.second);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
}

GLuint Texture::getTextureID(GLuint clusterID) {
	std::unordered_map<GLuint, GLuint>::iterator it = IDs.find(clusterID);
	if (it != IDs.end()) {
		return it->second;
	}
	
	return 0;
}

GLuint Texture::getVertexArrayObject(Window* win) {
	std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
	}
	
	return 0;
}

void Texture::render(Shader* shader, Window* window, TextureManager* textureManager) {
	use(shader, window, textureManager);

	GLuint VAO = getVertexArrayObject(window);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Texture::prepareContent(Window* win, TextureManager* textureManager) {
    win->makeCurrent();

    GLuint clusterID = win->getClusterID();
    generateBuffers(clusterID);
    generateTexture(clusterID);
    generateVertexArrayObject(clusterID, win);
}

void Texture::copy(const Texture& other) {
    width = other.width;
    height = other.height;
    depth = other.depth;
    textureName = other.textureName;
    shaderName = other.shaderName;
    bufferObjects = other.bufferObjects;
    VAOs = other.VAOs;
    IDs = other.IDs;
    if (other.image != NULL) {
        image = new GLubyte[width * height * depth];
        std::copy(other.image, other.image + (width * height * depth), image);
    } else {
        image = NULL;
    }
}

void Texture::move(Texture& other) {
    width = other.width;
    height = other.height;
    depth = other.depth;
    textureName = std::move(other.textureName);
    shaderName = std::move(other.shaderName);
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
	shaderName = "imageRender";
	bufferObjects.clear();
	VAOs.clear();
	IDs.clear();
	image = NULL;
}

void Texture::destroy() {
    if (image != NULL) {
        delete[] image;
    }
}

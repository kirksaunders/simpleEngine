#include <SOIL/SOIL.h>

#include "render_base/texture.hpp"
#include "render_base/texturemanager.hpp"

using namespace Render3D;

std::unordered_map<Window*, GLuint> Texture::VAOs;

Texture::Texture(int w, int h) {
	width = w;
	height = h;
	textureName = "tex";
	shaderName = "imageRender";
	generateBuffers();

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);	

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(char const *filePath) {
	textureName = "tex";
	shaderName = "imageRender";
	generateBuffers();

	unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);
	std::cout << filePath << std::endl;
	std::cout << SOIL_last_result() << std::endl;
	std::cout << width << " x " << height << std::endl;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);	

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
}

Texture::Texture(const TextureBuffer& buff) {
    textureName = "tex";
    shaderName = "imageRender";
    generateBuffers();

    width = buff.getWidth();
    height = buff.getHeight();

    int depth = buff.getDepth();
    if (depth != 3 && depth != 4) {
        // error
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if (depth == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buff.getAddress());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff.getAddress());
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadFromFile(char const *filePath) {
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
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

void Texture::generateBuffers() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);

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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Texture::getVertexArrayObject(Window* win) {
	std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
	}
	
	GLuint VAO;
	win->activate();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
	return VAO;
}

void Texture::setName(char const* name) {
	textureName = name;
}

char const* Texture::getName() {
	return textureName;
}

void Texture::setShader(char const *name) {
	shaderName = name;
}

char const* Texture::getShader() {
	return shaderName;
}

void Texture::use(Shader& shader) {
	int location = getLocation();
	if (location == -1) {
		location = useNextLocation();
		std::cout << "using next location " << location << " for " << textureID << std::endl;
	}
	shader.setVariable(textureName, location);
}

void Texture::use(Shader& shader, const char* name) {
	int location = getLocation();
	if (location == -1) {
		location = useNextLocation();
	}
	shader.setVariable(name, location);
}

int Texture::getLocation() {
	for (int i = 0; i < textureManager.getNumberTextures(); i++) {
		if (textureManager.getActiveTexture(i) == textureID) {
			return i;
		}
	}

	return -1;
}

int Texture::useNextLocation() {
	int location = textureManager.getNumberTextures();
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, textureID);
	textureManager.setActive(textureID, location);
	glActiveTexture(GL_TEXTURE0);
	return location;
}

void Texture::render(Shader& shader) {
	use(shader);

	GLuint VAO = getVertexArrayObject(Window::getCurrent());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

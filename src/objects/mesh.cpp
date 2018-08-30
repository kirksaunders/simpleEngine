#include "objects/mesh.hpp"

using namespace Render3D;
using namespace Math3D;

Mesh::Mesh(const std::vector<Vector4>& vertices, const std::vector<Vector4>& normals,
            const std::vector<TextureCoord>& texCoords, const std::vector<TextureData>& textures,
            const std::vector<GLuint>& indices) {
    this->textures = textures; // creates copy of textures vector, which obeys the const qualifier of argument
    numVertices = indices.size();

    // generate and load buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);

    unsigned int numIndices = indices.size();
    GLfloat* verticesNew = new GLfloat[3 * numIndices];

    for (int i = 0; i < numIndices; i++) {
        Vector4 vert = vertices[indices[i]];

        verticesNew[i * 3] = vert[0];
        verticesNew[i * 3 + 1] = vert[1];
        verticesNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numIndices, verticesNew, GL_STATIC_DRAW);
    delete[] verticesNew;

    GLfloat* normalsNew = new GLfloat[3 * numIndices];

    for (int i = 0; i < numIndices; i++) {
        Vector4 vert = normals[indices[i]];

        normalsNew[i * 3] = vert[0];
        normalsNew[i * 3 + 1] = vert[1];
        normalsNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numIndices, normalsNew, GL_STATIC_DRAW);
    delete[] normalsNew;

    GLfloat* texCoordsNew = new GLfloat[2 * numIndices];

    for (int i = 0; i < numIndices; i++) {
        TextureCoord tex = texCoords[indices[i]];

        texCoordsNew[i * 2] = tex.x;
        texCoordsNew[i * 2 + 1] = tex.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * numIndices, texCoordsNew, GL_STATIC_DRAW);
    delete[] texCoordsNew;

	glBindBuffer(GL_ARRAY_BUFFER, 0); // make sure no buffer is bound
}

GLuint Mesh::getVertexArrayObject(Window* win) {
	std::map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
	}

	win->activate();
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
	return VAO;
}

void Mesh::bindTextures(Shader& shader) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < textures.size(); i++) {
		TextureData tex = textures[i];
		std::string number;
		std::string name = tex.type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr);
			diffuseNr++;
		} else if (name == "texture_specular") {
			number = std::to_string(specularNr);
			specularNr++;
		}
		Texture* texture = textureManager.getTexture(tex.fullPath.c_str());
		texture->use(shader, (name + number).c_str());
	}
}

void Mesh::unbindTextures(Shader& shader) {
	Texture* defaultTex = textureManager.getTexture("defaultTexture");
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < textures.size(); i++) {
		TextureData tex = textures[i];
		std::string number;
		std::string name = tex.type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr);
			diffuseNr++;
		} else if (name == "texture_specular") {
			number = std::to_string(specularNr);
			specularNr++;
		}
		defaultTex->use(shader, (name + number).c_str());
	}
}

void Mesh::render(Shader& shader) {
	bindTextures(shader);

	GLuint VAO = getVertexArrayObject(Window::getCurrent());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	
	unbindTextures(shader);
}

int Mesh::getVertexCount() {
	return numVertices;
}

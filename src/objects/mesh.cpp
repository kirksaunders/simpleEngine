#include "objects/mesh.hpp"

using namespace Render3D;
using namespace Math3D;

Mesh::Mesh(std::vector<Vector4> vertices, std::vector<Vector4> normals, std::vector<TextureCoord> texCoords, std::vector<TextureData> textures, std::vector<GLuint> indices) {
	this->vertices = vertices;
	this->normals = normals;
	this->texCoords = texCoords;
	this->textures = textures;
	this->indices = indices;

	generateBuffers();
}

void Mesh::generateBuffers() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);

    {
        GLfloat verticesNew[3 * indices.size()];

        for (int i = 0; i < indices.size(); i++) {
            Vector4 vert = vertices[indices[i]];

            verticesNew[i * 3] = vert[0];
            verticesNew[i * 3 + 1] = vert[1];
            verticesNew[i * 3 + 2] = vert[2];
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesNew), verticesNew, GL_STATIC_DRAW);
    }

    {
        GLfloat normalsNew[3 * indices.size()];

        for (int i = 0; i < indices.size(); i++) {
            Vector4 vert = normals[indices[i]];

            normalsNew[i * 3] = vert[0];
            normalsNew[i * 3 + 1] = vert[1];
            normalsNew[i * 3 + 2] = vert[2];
        }

        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normalsNew), normalsNew, GL_STATIC_DRAW);
    }

    {
        GLfloat texCoordsNew[2 * indices.size()];

        for (int i = 0; i < indices.size(); i++) {
            TextureCoord tex = texCoords[indices[i]];

            texCoordsNew[i * 2] = tex.x;
            texCoordsNew[i * 2 + 1] = tex.y;
        }

        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordsNew), texCoordsNew, GL_STATIC_DRAW);
    }

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glDrawArrays(GL_TRIANGLES, 0, indices.size());
    
    unbindTextures(shader);
}

/*void Mesh::updateVertices() {
	GLfloat verticesNew[3 * indices.size()];

	for (int i = 0; i < indices.size(); i++) {
		Vector4 vert = vertices[indices[i]];

		vert = vert * size;

		verticesNew[i * 3] = vert[0];
		verticesNew[i * 3 + 1] = vert[1];
		verticesNew[i * 3 + 2] = vert[2];
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesNew), verticesNew);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}

void Mesh::updateNormals() {
	Matrix4x4 rotation = cframe.rotation();

	GLfloat normalsNew[3 * indices.size()];

	for (int i = 0; i < indices.size(); i++) {
		Vector4 norm = normals[indices[i]];

		norm = rotation * norm;

		normalsNew[i * 3] = norm[0];
		normalsNew[i * 3 + 1] = norm[1];
		normalsNew[i * 3 + 2] = norm[2];
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(normalsNew), normalsNew);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}*/

int Mesh::getVertexCount() {
	return indices.size();
}

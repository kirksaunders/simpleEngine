#include "objects/mesh.hpp"

#include "render_base/texturemanager.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

Mesh::Mesh(const std::vector<Vector4>& verts, const std::vector<Vector4>& norms,
           const std::vector<TextureCoord>& texCs, const std::vector<TextureData>& texs,
           const std::vector<GLuint>& inds) {
    vertices = verts;
    normals = norms;
    texCoords = texCs;
    textures = texs;
    indices = inds;
    numVertices = indices.size();
}

void Mesh::generateBuffers(GLuint clusterID) {
    std::unordered_map<GLuint, BufferTriple>::iterator it = BufferObjects.find(clusterID);
	if (it != BufferObjects.end()) {
		return;
	}

    GLuint VBO, NBO, TBO;

    // generate and load buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);

    GLfloat* verticesNew = new GLfloat[3 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        Vector4 vert = vertices[indices[i]];

        verticesNew[i * 3] = vert[0];
        verticesNew[i * 3 + 1] = vert[1];
        verticesNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numVertices, verticesNew, GL_STATIC_DRAW);
    delete[] verticesNew;

    GLfloat* normalsNew = new GLfloat[3 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        Vector4 vert = normals[indices[i]];

        normalsNew[i * 3] = vert[0];
        normalsNew[i * 3 + 1] = vert[1];
        normalsNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numVertices, normalsNew, GL_STATIC_DRAW);
    delete[] normalsNew;

    GLfloat* texCoordsNew = new GLfloat[2 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        TextureCoord tex = texCoords[indices[i]];

        texCoordsNew[i * 2] = tex.x;
        texCoordsNew[i * 2 + 1] = tex.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * numVertices, texCoordsNew, GL_STATIC_DRAW);
    delete[] texCoordsNew;

	glBindBuffer(GL_ARRAY_BUFFER, 0); // make sure no buffer is bound

    BufferObjects.insert(std::pair<GLuint, BufferTriple>(clusterID, BufferTriple(VBO, NBO, TBO)));
}

void Mesh::generateVertexArrayObject(GLuint clusterID, Window* win) {
    std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return;
	}

    std::unordered_map<GLuint, BufferTriple>::iterator it2 = BufferObjects.find(clusterID);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

    // Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, std::get<0>(it2->second));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Bind NBO
	glBindBuffer(GL_ARRAY_BUFFER, std::get<1>(it2->second));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Bind TBO
	glBindBuffer(GL_ARRAY_BUFFER, std::get<2>(it2->second));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
}

GLuint Mesh::getVertexArrayObject(Window* win) {
	std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
	}

    return -1;
}

int Mesh::getVertexCount() {
	return numVertices;
}

void Mesh::bindTextures(Shader* shader, Window* win, TextureManager* textureManager) {
	GLuint diffuseNr = 0;
	GLuint specularNr = 0;
    TextureData* tex;
	for (GLuint i = 0; i < textures.size(); i++) {
		tex = &textures[i];
		std::string number;
		std::string name = tex->type;
		if (name == "texture_diffuse") {
			number = std::to_string(++diffuseNr);
		} else if (name == "texture_specular") {
			number = std::to_string(++specularNr);
		}
		tex->tex->use(shader, win, textureManager, (name + number).c_str());
	}
}

void Mesh::unbindTextures(Shader* shader, Window* win, TextureManager* textureManager) {
	Texture* defaultTex = textureManager->getTexture("defaultTexture");
	GLuint diffuseNr = 0;
	GLuint specularNr = 0;
    TextureData* tex;
	for (GLuint i = 0; i < textures.size(); i++) {
		tex = &textures[i];
		std::string number;
		std::string name = tex->type;
		if (name == "texture_diffuse") {
			number = std::to_string(++diffuseNr);
		} else if (name == "texture_specular") {
			number = std::to_string(++specularNr);
		}
		defaultTex->use(shader, win, textureManager, (name + number).c_str());
	}
}

void Mesh::render(Shader* const shader, Window* const win, TextureManager* const textureManager) {
	bindTextures(shader, win, textureManager);

	GLuint VAO = getVertexArrayObject(win);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	
	unbindTextures(shader, win, textureManager);
}

void Mesh::prepareContent(Window* win, TextureManager* textureManager) {
    win->makeCurrent();

    GLuint clusterID = win->getClusterID();
    generateBuffers(clusterID);
    generateVertexArrayObject(clusterID, win);

    for (unsigned int i = 0; i < textures.size(); i++) {
        textures[i].tex->prepareContent(win, textureManager);
    }
}

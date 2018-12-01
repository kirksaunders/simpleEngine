#include "objects/cuboid.hpp"

#include "render_base/shader.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

Cuboid::Cuboid() : Primitive3D() {
    vertices[0] = Vector4(-0.5, 0.5, 0.5);
	vertices[1] = Vector4(-0.5, 0.5, -0.5);
	vertices[2] = Vector4(0.5, 0.5, -0.5);
	vertices[3] = Vector4(0.5, 0.5, 0.5);
	vertices[4] = Vector4(-0.5, -0.5, 0.5);
	vertices[5] = Vector4(-0.5, -0.5, -0.5);
	vertices[6] = Vector4(0.5, -0.5, -0.5);
	vertices[7] = Vector4(0.5, -0.5, 0.5);

    normals[0] = Vector4(0, 1, 0); // Up
	normals[1] = Vector4(0, -1, 0); // Down
	normals[2] = Vector4(1, 0, 0); // Right
	normals[3] = Vector4(-1, 0, 0); // Left
	normals[4] = Vector4(0, 0, -1); // Forward
	normals[5] = Vector4(0, 0, 1); // Backward

    // Top Face
    vertexIndices[0] = 0;
	vertexIndices[1] = 1;
	vertexIndices[2] = 2;
	vertexIndices[3] = 0;
	vertexIndices[4] = 2;
	vertexIndices[5] = 3;

	for (int i = 0; i < 6; i++) {
		normalIndices[i] = 0;
	}

	// Bottom Face
	vertexIndices[6] = 5;
	vertexIndices[7] = 4;
	vertexIndices[8] = 7;
	vertexIndices[9] = 5;
	vertexIndices[10] = 7;
	vertexIndices[11] = 6;

	for (int i = 6; i < 12; i++) {
		normalIndices[i] = 1;
	}

	// Right Face
	vertexIndices[12] = 2;
	vertexIndices[13] = 6;
	vertexIndices[14] = 7;
	vertexIndices[15] = 2;
	vertexIndices[16] = 7;
	vertexIndices[17] = 3;

	for (int i = 12; i < 18; i++) {
		normalIndices[i] = 2;
	}

	// Left Face
	vertexIndices[18] = 0;
	vertexIndices[19] = 4;
	vertexIndices[20] = 5;
	vertexIndices[21] = 0;
	vertexIndices[22] = 5;
	vertexIndices[23] = 1;

	for (int i = 18; i < 24; i++) {
		normalIndices[i] = 3;
	}

	// Front Face
	vertexIndices[24] = 1;
	vertexIndices[25] = 5;
	vertexIndices[26] = 6;
	vertexIndices[27] = 1;
	vertexIndices[28] = 6;
	vertexIndices[29] = 2;

	for (int i = 24; i < 30; i++) {
		normalIndices[i] = 4;
	}

	// Back Face
	vertexIndices[30] = 3;
	vertexIndices[31] = 7;
	vertexIndices[32] = 4;
	vertexIndices[33] = 3;
	vertexIndices[34] = 4;
	vertexIndices[35] = 0;

	for (int i = 30; i < 36; i++) {
		normalIndices[i] = 5;
	}

}

void Cuboid::generateBuffers(GLuint clusterID) {
    std::unordered_map<GLuint, BufferPair>::iterator it = BufferObjects.find(clusterID);
	if (it != BufferObjects.end()) {
		return;
	}

    GLfloat verts[108];
    GLfloat norms[108];

	for (int i = 0; i < 36; i++) {
		Vector4 temp = vertices[vertexIndices[i]];

		verts[i * 3] = temp[0];
		verts[i * 3 + 1] = temp[1];
		verts[i * 3 + 2] = temp[2];

        temp = normals[normalIndices[i]];

		norms[i * 3] = temp[0];
		norms[i * 3 + 1] = temp[1];
		norms[i * 3 + 2] = temp[2];
	}

    // Generate GL Buffers
    GLuint VBO, NBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(norms), norms, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

    BufferObjects.insert(std::pair<GLuint, BufferPair>(clusterID, BufferPair(VBO, NBO)));
}

void Cuboid::generateVertexArrayObject(GLuint clusterID, Window* win) {
    std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return;
	}

    std::unordered_map<GLuint, BufferPair>::iterator it2 = BufferObjects.find(clusterID);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

    // Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, it2->second.first);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Bind NBO
	glBindBuffer(GL_ARRAY_BUFFER, it2->second.second);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
}

GLuint Cuboid::getVertexArrayObject(Window* win) {
	std::unordered_map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
	}

    return 0;
}

int Cuboid::getVertexCount() {
	return 36;
}

void Cuboid::render(Shader* const shader, Window* const win, TextureManager* const textureManager) {
	Matrix4x4 rotation = cframe.rotation();

	shader->setVariable(win, "modelCFrame", cframe);
	shader->setVariable(win, "modelRotation", rotation);
	shader->setVariable(win, "modelSize", size);
	shader->setVariable(win, "modelColor", color);

	GLuint VAO = getVertexArrayObject(win);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cuboid::prepareContent(Window* win, TextureManager* textureManager) {
    win->makeCurrent();

    GLuint clusterID = win->getClusterID();
    generateBuffers(clusterID);
    generateVertexArrayObject(clusterID, win);
}

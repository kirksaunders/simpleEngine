#include "objects/cuboid.hpp"

using namespace Render3D;
using namespace Math3D;

void Cuboid::setNormals() {
	Vector4 norms[6];

	norms[0] = Vector4(0, 1, 0); // Up
	norms[1] = Vector4(0, -1, 0); // Down
	norms[2] = Vector4(1, 0, 0); // Right
	norms[3] = Vector4(-1, 0, 0); // Left
	norms[4] = Vector4(0, 0, -1); // Forward
	norms[5] = Vector4(0, 0, 1); // Backward

	for (int i = 0; i < 36; i++) {
		Vector4 norm = norms[normalIndices[i]];

		normals[i * 3] = norm[0];
		normals[i * 3 + 1] = norm[1];
		normals[i * 3 + 2] = norm[2];
	}
}

void Cuboid::setVertices() {
	Vector4 verts[8];

	verts[0] = Vector4(-0.5, 0.5, 0.5);
	verts[1] = Vector4(-0.5, 0.5, -0.5);
	verts[2] = Vector4(0.5, 0.5, -0.5);
	verts[3] = Vector4(0.5, 0.5, 0.5);

	verts[4] = Vector4(-0.5, -0.5, 0.5);
	verts[5] = Vector4(-0.5, -0.5, -0.5);
	verts[6] = Vector4(0.5, -0.5, -0.5);
	verts[7] = Vector4(0.5, -0.5, 0.5);

	for (int i = 0; i < 36; i++) {
		Vector4 vec = verts[vertexIndices[i]];

		vertices[i * 3] = vec[0];
		vertices[i * 3 + 1] = vec[1];
		vertices[i * 3 + 2] = vec[2];
	}
}

Cuboid::Cuboid() {
    shaderName = "defaultPerspective";

    generateBuffers();
}

void Cuboid::generateBuffers() {
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

	setNormals();
	setVertices();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Cuboid::getVertexArrayObject(Window* win) {
    std::map<Window*, GLuint>::iterator it = VAOs.find(win);
	if (it != VAOs.end()) {
		return it->second;
    }

    GLuint VAO;
    win->activate();
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	VAOs.insert(std::pair<Window*, GLuint>(win, VAO));
}

/*std::vector<Face> Cuboid::getFaces() {
	return faces;
}

void Cuboid::calculateFaces() {
	Vector4 size2 = size / 2;

	Vector4 v1 = cframe * Vector4(-size2[0], size2[1], size2[2]);
	Vector4 v2 = cframe * Vector4(-size2[0], size2[1], -size2[2]);
	Vector4 v3 = cframe * Vector4(size2[0], size2[1], -size2[2]);
	Vector4 v4 = cframe * Vector4(size2[0], size2[1], size2[2]);

	Vector4 v5 = cframe * Vector4(-size2[0], -size2[1], size2[2]);
	Vector4 v6 = cframe * Vector4(-size2[0], -size2[1], -size2[2]);
	Vector4 v7 = cframe * Vector4(size2[0], -size2[1], -size2[2]);
	Vector4 v8 = cframe * Vector4(size2[0], -size2[1], size2[2]);

	if (faces.size() == 0) {
		std::vector<Vector4> f1 = {v1, v2, v3, v4}; // Top
		faces.push_back(Face(f1, color));

		std::vector<Vector4> f2 = {v6, v5, v8, v7}; // Bottom
		faces.push_back(Face(f2, color));

		std::vector<Vector4> f3 = {v3, v7, v8, v4}; // Right
		faces.push_back(Face(f3, color));

		std::vector<Vector4> f4 = {v1, v5, v6, v2}; // Left
		faces.push_back(Face(f4, color));

		std::vector<Vector4> f5 = {v2, v6, v7, v3}; // Front
		faces.push_back(Face(f5, color));

		std::vector<Vector4> f6 = {v4, v8, v5, v1}; // Back
		faces.push_back(Face(f6, color));
	} else {
		std::vector<Vector4> f1 = {v1, v2, v3, v4}; // Top
		faces[0].update(f1, color);

		std::vector<Vector4> f2 = {v6, v5, v8, v7}; // Bottom
		faces[1].update(f2, color);

		std::vector<Vector4> f3 = {v3, v7, v8, v4}; // Right
		faces[2].update(f3, color);

		std::vector<Vector4> f4 = {v1, v5, v6, v2}; // Left
		faces[3].update(f4, color);

		std::vector<Vector4> f5 = {v2, v6, v7, v3}; // Front
		faces[4].update(f5, color);

		std::vector<Vector4> f6 = {v4, v8, v5, v1}; // Back
		faces[5].update(f6, color);
	}
}*/

/*GLuint Cuboid::getVertexArrayObject() {
    GLuint VAO;
    std::map<Window*, GLuint>::iterator it = VAOs.find(Window::getCurrent());
	if (it != VAOs.end()) {
		VAO = it->second;
    }
    return VAO;
}*/

int Cuboid::getVertexCount() {
	return 36;
}

void Cuboid::render(Shader& shader) {
	Matrix4x4 rotation = cframe.rotation();

	shader.setVariable("modelCFrame", cframe);
	shader.setVariable("modelRotation", rotation);
	shader.setVariable("modelSize", size);
	shader.setVariable("modelColor", color);

    GLuint VAO = getVertexArrayObject(Window::getCurrent());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

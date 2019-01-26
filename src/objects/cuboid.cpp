#include "objects/cuboid.hpp"

#include "render_base/shader.hpp"

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
    vertexIndices[0] = 2;
    vertexIndices[1] = 1;
    vertexIndices[2] = 0;
    vertexIndices[3] = 3;
    vertexIndices[4] = 2;
    vertexIndices[5] = 0;

    for (int i = 0; i < 6; ++i) {
        normalIndices[i] = 0;
    }

    // Bottom Face
    vertexIndices[6] = 4;
    vertexIndices[7] = 5;
    vertexIndices[8] = 6;
    vertexIndices[9] = 4;
    vertexIndices[10] = 6;
    vertexIndices[11] = 7;

    for (int i = 6; i < 12; ++i) {
        normalIndices[i] = 1;
    }

    // Right Face
    vertexIndices[12] = 2;
    vertexIndices[13] = 3;
    vertexIndices[14] = 7;
    vertexIndices[15] = 2;
    vertexIndices[16] = 7;
    vertexIndices[17] = 6;

    for (int i = 12; i < 18; ++i) {
        normalIndices[i] = 2;
    }

    // Left Face
    vertexIndices[18] = 0;
    vertexIndices[19] = 1;
    vertexIndices[20] = 5;
    vertexIndices[21] = 0;
    vertexIndices[22] = 5;
    vertexIndices[23] = 4;

    for (int i = 18; i < 24; ++i) {
        normalIndices[i] = 3;
    }

    // Front Face
    vertexIndices[24] = 0;
    vertexIndices[25] = 4;
    vertexIndices[26] = 7;
    vertexIndices[27] = 0;
    vertexIndices[28] = 7;
    vertexIndices[29] = 3;

    for (int i = 24; i < 30; ++i) {
        normalIndices[i] = 5;
    }

    // Back Face
    vertexIndices[30] = 1;
    vertexIndices[31] = 2;
    vertexIndices[32] = 6;
    vertexIndices[33] = 1;
    vertexIndices[34] = 6;
    vertexIndices[35] = 5;

    for (int i = 30; i < 36; ++i) {
        normalIndices[i] = 4;
    }
}

BufferObject Cuboid::generateBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            ++(bufferObjects[i].second.useCount);
            return bufferObjects[i].second;
        }
    }

    GLfloat data[36 * 3 * 2];
    for (unsigned int i = 0; i < 36; ++i) {
        unsigned int ind = vertexIndices[i];
        data[i*6] =     vertices[ind][0];
        data[i*6 + 1] = vertices[ind][1];
        data[i*6 + 2] = vertices[ind][2];

        ind = normalIndices[i];
        data[i*6 + 3] = normals[ind][0];
        data[i*6 + 4] = normals[ind][1];
        data[i*6 + 5] = normals[ind][2];
    }

    // Generate GL Buffers
    BufferObject buffer;

    glGenBuffers(1, &buffer.id);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    buffer.useCount = 1;
    bufferObjects.push_back(std::pair<GLuint, BufferObject>(clusterID, buffer));

    return buffer;
}

void Cuboid::destroyBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            if (--(bufferObjects[i].second.useCount) == 0) {
                glDeleteBuffers(1, &bufferObjects[i].second.id);
                std::swap(bufferObjects[i], bufferObjects.back());
                bufferObjects.pop_back();
            }
            return;
        }
    }
}

void Cuboid::generateVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return;
        }
    }

    BufferObject buffer = generateBuffers(win.getClusterID());

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id);

    // Set Vertex Info
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    // Set Normal Info
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    VAOs.push_back(std::pair<const Window*, GLuint>(&win, VAO));
}

void Cuboid::destroyVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            glDeleteVertexArrays(1, &VAOs[i].second);
            std::swap(VAOs[i], VAOs.back());
            VAOs.pop_back();
            return;
        }
    }
}

GLuint Cuboid::getVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return VAOs[i].second;
        }
    }

    return 0;
}

void Cuboid::render(Window& win, TextureManager& textureManager) {
    applyVariables(win);

    glBindVertexArray(getVertexArrayObject(win));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cuboid::prepareContent(Window& win, TextureManager& textureManager) {
    generateVertexArrayObject(win); // this function makes a call to generateBuffers
}

void Cuboid::destroyContent(Window& win, TextureManager& textureManager) {
    destroyVertexArrayObject(win);
    destroyBuffers(win.getClusterID());
}

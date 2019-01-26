#include "objects/mesh.hpp"

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

DoubleBufferObject Mesh::generateBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            ++(bufferObjects[i].second.useCount);
            return bufferObjects[i].second;
        }
    }

    DoubleBufferObject buffers;

    // generate and load buffers
    glGenBuffers(1, &buffers.firstID);
    glGenBuffers(1, &buffers.secondID);

    GLfloat* data = new GLfloat[8 * numVertices];

    for (int i = 0; i < vertices.size(); ++i) {
        data[i*8] =     vertices[i][0];
        data[i*8 + 1] = vertices[i][1];
        data[i*8 + 2] = vertices[i][2];

        data[i*8 + 3] = normals[i][0];
        data[i*8 + 4] = normals[i][1];
        data[i*8 + 5] = normals[i][2];

        data[i*8 + 6] = texCoords[i].x;
        data[i*8 + 7] = texCoords[i].y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers.firstID);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat) * vertices.size(), data, GL_STATIC_DRAW);
    delete[] data;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.secondID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    buffers.useCount = 1;
    bufferObjects.push_back(std::pair<GLuint, DoubleBufferObject>(clusterID, buffers));

    return buffers;
}

void Mesh::destroyBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            if (--(bufferObjects[i].second.useCount) == 0) {
                glDeleteBuffers(1, &bufferObjects[i].second.firstID);
                glDeleteBuffers(1, &bufferObjects[i].second.secondID);
                std::swap(bufferObjects[i], bufferObjects.back());
                bufferObjects.pop_back();
            }
            return;
        }
    }
}

void Mesh::generateVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return;
        }
    }

    DoubleBufferObject buffers = generateBuffers(win.getClusterID());

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Bind Buffers
    glBindBuffer(GL_ARRAY_BUFFER, buffers.firstID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.secondID);

    // Set Vertex Info
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

    // Set Normal Info
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    // Set UV Coordinate Info
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    // Unbind Buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    VAOs.push_back(std::pair<const Window*, GLuint>(&win, VAO));
}

void Mesh::destroyVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            glDeleteVertexArrays(1, &VAOs[i].second);
            std::swap(VAOs[i], VAOs.back());
            VAOs.pop_back();
            return;
        }
    }
}

GLuint Mesh::getVertexArrayObject(Window& win) {
    for (unsigned int i = 0; i < VAOs.size(); ++i) {
        if (VAOs[i].first == &win) {
            return VAOs[i].second;
        }
    }

    return 0;
}

GLuint Mesh::getElementArrayBuffer(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            return bufferObjects[i].second.secondID;
        }
    }

    return 0;
}

int Mesh::getVertexCount() {
    return numVertices;
}

void Mesh::bindTextures(Shader& shader, Window& win, TextureManager& textureManager) {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    TextureData* tex;
    for (unsigned int i = 0; i < textures.size(); ++i) {
        tex = &textures[i];
        if (tex->type == "texture_diffuse") {
            tex->tex->useDiffuse(shader, win, textureManager, diffuseNr);
            diffuseNr++;
        } else if (tex->type == "texture_specular") {
            tex->tex->useSpecular(shader, win, textureManager, specularNr);
            specularNr++;
        }
    }
}

void Mesh::unbindTextures(Shader& shader, Window& win, TextureManager& textureManager) {
    Texture& defaultTex = textureManager.getDefaultTexture();
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    TextureData* tex;
    for (unsigned int i = 0; i < textures.size(); ++i) {
        tex = &textures[i];
        if (tex->type == "texture_diffuse") {
            defaultTex.useDiffuse(shader, win, textureManager, diffuseNr);
            diffuseNr++;
        } else if (tex->type == "texture_specular") {
            defaultTex.useSpecular(shader, win, textureManager, specularNr);
            specularNr++;
        }
    }
}

void Mesh::render(Shader& shader, Window& win, TextureManager& textureManager) {
    bindTextures(shader, win, textureManager);

    glBindVertexArray(getVertexArrayObject(win));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getElementArrayBuffer(win.getClusterID()));
    //glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, (GLvoid*)0);
    
    unbindTextures(shader, win, textureManager);
}

void Mesh::prepareContent(Window& win, TextureManager& textureManager) {
    generateVertexArrayObject(win);
}

void Mesh::destroyContent(Window& win, TextureManager& textureManager) {
    destroyVertexArrayObject(win);
    destroyBuffers(win.getClusterID());
}

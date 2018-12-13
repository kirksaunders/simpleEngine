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

BufferTriple Mesh::generateBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            ++(bufferObjects[i].second.useCount);
            return bufferObjects[i].second;
        }
    }

    BufferTriple triple;

    // generate and load buffers
    glGenBuffers(1, &triple.first);
    glGenBuffers(1, &triple.second);
    glGenBuffers(1, &triple.third);

    GLfloat* verticesNew = new GLfloat[3 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        Vector4 vert = vertices[indices[i]];

        verticesNew[i * 3] = vert[0];
        verticesNew[i * 3 + 1] = vert[1];
        verticesNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, triple.first);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numVertices, verticesNew, GL_STATIC_DRAW);
    delete[] verticesNew;

    GLfloat* normalsNew = new GLfloat[3 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        Vector4 vert = normals[indices[i]];

        normalsNew[i * 3] = vert[0];
        normalsNew[i * 3 + 1] = vert[1];
        normalsNew[i * 3 + 2] = vert[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, triple.second);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * numVertices, normalsNew, GL_STATIC_DRAW);
    delete[] normalsNew;

    GLfloat* texCoordsNew = new GLfloat[2 * numVertices];

    for (int i = 0; i < numVertices; i++) {
        TextureCoord tex = texCoords[indices[i]];

        texCoordsNew[i * 2] = tex.x;
        texCoordsNew[i * 2 + 1] = tex.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, triple.third);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * numVertices, texCoordsNew, GL_STATIC_DRAW);
    delete[] texCoordsNew;

    glBindBuffer(GL_ARRAY_BUFFER, 0); // make sure no buffer is bound

    triple.useCount = 1;
    bufferObjects.push_back(std::pair<GLuint, BufferTriple>(clusterID, triple));

    return triple;
}

void Mesh::destroyBuffers(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            if (--(bufferObjects[i].second.useCount) == 0) {
                glDeleteBuffers(1, &bufferObjects[i].second.first);
                glDeleteBuffers(1, &bufferObjects[i].second.second);
                glDeleteBuffers(1, &bufferObjects[i].second.third);
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

    BufferTriple buffers = generateBuffers(win.getClusterID());

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, buffers.first);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Bind NBO
    glBindBuffer(GL_ARRAY_BUFFER, buffers.second);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Bind TBO
    glBindBuffer(GL_ARRAY_BUFFER, buffers.third);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

int Mesh::getVertexCount() {
    return numVertices;
}

void Mesh::bindTextures(Shader& shader, Window& win, TextureManager& textureManager) {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    TextureData* tex;
    for (unsigned int i = 0; i < textures.size(); i++) {
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
    for (unsigned int i = 0; i < textures.size(); i++) {
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
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    
    unbindTextures(shader, win, textureManager);
}

void Mesh::prepareContent(Window& win, TextureManager& textureManager) {
    generateVertexArrayObject(win);
}

void Mesh::destroyContent(Window& win, TextureManager& textureManager) {
    destroyVertexArrayObject(win);
    destroyBuffers(win.getClusterID());
}

#include <iostream>
#include <algorithm>

#include "render_base/exception.hpp"
#include "render_base/texturemanager.hpp"
#include "render_base/texturebuffer.hpp"
#include "render_base/color.hpp"

using namespace Render3D;

const std::string TextureManager::diffNames[TextureManager::MAX_MATERIAL_TEXTURES] = {
    "texture_diffuse1",
    "texture_diffuse2",
    "texture_diffuse3",
    "texture_diffuse4",
    "texture_diffuse5"
};

const std::string TextureManager::specNames[TextureManager::MAX_MATERIAL_TEXTURES] = {
    "texture_specular1",
    "texture_specular2",
    "texture_specular3",
    "texture_specular4",
    "texture_specular5"
};

TextureManager::TextureManager() {
    TextureBuffer buff(1, 1, 4);
    buff.setPixel(0, 0, Color(0, 0, 0, 0));

    defaultTexture = Texture(buff);
}

void TextureManager::addWindow(Window& win) {
    for (unsigned int i = 0; i < activeTexturesByWindow.size(); ++i) {
        if (activeTexturesByWindow[i].first == &win) {
            return;
        }
    }

    activeTexturesByWindow.push_back(std::pair<const Window*, std::vector<GLuint> >(&win, std::vector<GLuint>()));
}

void TextureManager::removeWindow(Window& win) {
    for (unsigned int i = 0; i < activeTexturesByWindow.size(); ++i) {
        if (activeTexturesByWindow[i].first == &win) {
            std::swap(activeTexturesByWindow[i], activeTexturesByWindow.back());
            activeTexturesByWindow.pop_back();
            return;
        }
    }
}

int TextureManager::getTextureLocation(GLuint texID, Window& win) {
    for (unsigned int i = 0; i < activeTexturesByWindow.size(); ++i) {
        if (activeTexturesByWindow[i].first == &win) {
            const std::vector<GLuint>* vec = &activeTexturesByWindow[i].second;
            for (unsigned int i2 = 0; i2 < vec->size(); ++i2) {
                if ((*vec)[i2] == texID) {
                    return i2;
                }
            }

            return -1;
        }
    }

    return -1;
}

int TextureManager::makeTextureActive(GLuint texID, Window& win) {
    for (unsigned int i = 0; i < activeTexturesByWindow.size(); ++i) {
        if (activeTexturesByWindow[i].first == &win) {
            std::vector<GLuint>* vec = &activeTexturesByWindow[i].second;
            for (unsigned int i2 = 0; i2 < vec->size(); ++i2) {
                if ((*vec)[i2] == 0) {
                    (*vec)[i2] = texID;
                    return i2;
                }
            }

            vec->push_back(texID);
            return vec->size() - 1;
        }
    }

    throw Exception("Unable to make texture active, Window has not been added to TextureManager list");
}

int TextureManager::makeTextureInactive(GLuint texID, Window& win) {
    for (unsigned int i = 0; i < activeTexturesByWindow.size(); ++i) {
        if (activeTexturesByWindow[i].first == &win) {
            std::vector<GLuint>* vec = &activeTexturesByWindow[i].second;
            for (unsigned int i2 = 0; i2 < vec->size(); ++i2) {
                if ((*vec)[i2] == 0) {
                    (*vec)[i2] = 0;
                    return i2;
                }
            }

            return -1;
        }
    }

    throw Exception("Unable to make texture inactive, Window has not been added to TextureManager list");
}

const std::string& TextureManager::getDiffuseName(unsigned int num) {
    return diffNames[num];
}

const std::string& TextureManager::getSpecularName(unsigned int num) {
    return specNames[num];
}

Texture& TextureManager::getDefaultTexture() {
    return defaultTexture;
}

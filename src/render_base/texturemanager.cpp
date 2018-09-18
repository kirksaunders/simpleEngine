#include "render_base/texturemanager.hpp"

using namespace Render3D;

TextureManager::TextureManager() {
	// nothing?
}

bool TextureManager::textureDoesExist(char const* textureName) const {
	return textures.count(std::string(textureName)) == 1;
}

void TextureManager::addWindow(Window* win) {
	std::map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(win);
	if (it != activeTexturesByWindow.end()) {
        // error
		return; // i guess for now
	} else {
		std::vector<GLuint> vec;
		activeTexturesByWindow.insert(std::pair<Window*, std::vector<GLuint> >(win, vec));
	}
}

Texture* TextureManager::getTexture(char const *textureName) {
	return &textures.at(std::string(textureName));
}

int TextureManager::getNumberTextures() const {
	std::map<Window*, std::vector<GLuint> >::const_iterator it = activeTexturesByWindow.find(Window::getCurrent());
	if (it != activeTexturesByWindow.end()) {
		return it->second.size();
	} else {
		return 0;
	}
}

void TextureManager::addTexture(char const* textureName, const Texture& texture) {
	std::string name(textureName);
	std::map<std::string, Texture>::iterator it = textures.find(name);
	if (it != textures.end()) {
		// texture with that name already exists, replace it with this one
		it->second = texture;
	} else {
		textures.insert(std::pair<std::string, Texture>(std::string(textureName), texture));
	}
}

GLuint TextureManager::getActiveTexture(int location) {
	std::map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(Window::getCurrent());
	if (it != activeTexturesByWindow.end()) {
		return it->second[location];
	} else {
		return 0;
	}
}

void TextureManager::setActive(GLuint texID, int location) {
	std::map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(Window::getCurrent());
	if (it != activeTexturesByWindow.end()) {
		if (location >= it->second.size()) {
			it->second.push_back(texID);
		} else {
			it->second[location] = texID;
		}
	} else {
        // error
		return; // for now
	}
}

namespace Render3D {
	TextureManager textureManager;
}

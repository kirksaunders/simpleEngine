#include "render_base/texturemanager.hpp"

#include <iostream>

using namespace Render3D;

TextureManager::TextureManager() {
	// nothing?
}

bool TextureManager::textureDoesExist(const std::string& textureName) const {
	return textures.count(textureName) == 1;
}

void TextureManager::addWindow(Window* win) {
	std::unordered_map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(win);
	if (it != activeTexturesByWindow.end()) {
        activeTexturesByWindow.erase(it);
	} else {
		// error
        return; // i guess for now
	}
}

void TextureManager::removeWindow(Window* win) {
	std::unordered_map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(win);
	if (it != activeTexturesByWindow.end()) {
        // error
		return; // i guess for now
	} else {
		std::vector<GLuint> vec;
		activeTexturesByWindow.insert(std::pair<Window*, std::vector<GLuint> >(win, vec));
	}
}

Texture* TextureManager::getTexture(const std::string& textureName) {
	return &textures.at(textureName);
}

int TextureManager::getNumberTextures(Window* win) const {
	std::unordered_map<Window*, std::vector<GLuint> >::const_iterator it = activeTexturesByWindow.find(win);
	if (it != activeTexturesByWindow.end()) {
		return it->second.size();
	} else {
		return 0;
	}
}

void TextureManager::addTexture(const std::string& textureName, Texture&& texture) {
	std::unordered_map<std::string, Texture>::iterator it = textures.find(textureName);
	if (it != textures.end()) {
		// texture with that name already exists, replace it with this one
		it->second = std::move(texture);
	} else {
		textures.insert(std::pair<std::string, Texture>(textureName, std::move(texture)));
	}
}

void TextureManager::addTexture(const std::string& textureName, const Texture& texture) {
	std::unordered_map<std::string, Texture>::iterator it = textures.find(textureName);
	if (it != textures.end()) {
		// texture with that name already exists, replace it with this one
		it->second = texture;
	} else {
		textures.insert(std::pair<std::string, Texture>(textureName, texture));
	}
}

GLuint TextureManager::getActiveTexture(int location, Window* win) {
	std::unordered_map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(win);
	if (it != activeTexturesByWindow.end()) {
		return it->second[location];
	} else {
		return 0;
	}
}

void TextureManager::setActive(GLuint texID, int location, Window* win) {
	std::unordered_map<Window*, std::vector<GLuint> >::iterator it = activeTexturesByWindow.find(win);
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

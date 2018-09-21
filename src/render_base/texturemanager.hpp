#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "render_base/texture.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	class TextureManager {
	 public:
		TextureManager();

		bool textureDoesExist(char const *textureName) const;

		void addWindow(Window* win);

		Texture* getTexture(char const *textureName);

		int getNumberTextures() const;

		void addTexture(char const *textureName, const Texture& texture);

		GLuint getActiveTexture(int location);

		void setActive(GLuint texID, int location);
	 private:
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<Window*, std::vector<GLuint> > activeTexturesByWindow;
	};

	extern TextureManager textureManager;
}

#endif
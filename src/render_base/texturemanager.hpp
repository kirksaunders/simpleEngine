#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <map>
#include <string>

#include "render_base/texture.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	class Texture;

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
		std::map<std::string, Texture> textures;
		std::map<Window*, std::vector<GLuint> > activeTexturesByWindow;
	};

	extern TextureManager textureManager;
}

#endif
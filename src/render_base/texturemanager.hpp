#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#define GLEW_STATIC

#include <string>
#include <vector>
#include <unordered_map>

#include <GLEW/glew.h>

#include "render_base/texture.hpp"

namespace Render3D {
    class Window; // forward declaration

	class TextureManager {
	 public:
		TextureManager();

		bool textureDoesExist(const std::string& textureName) const;

		void addWindow(Window* win);
		void removeWindow(Window* win);

		Texture* getTexture(const std::string& textureName);

		int getNumberTextures(Window* win) const;

		void addTexture(const std::string& textureName, Texture&& texture);
		void addTexture(const std::string& textureName, const Texture& texture);

		GLuint getActiveTexture(int location, Window* win);

		void setActive(GLuint texID, int location, Window* win);
	 private:
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<Window*, std::vector<GLuint> > activeTexturesByWindow;
	};
}

#endif
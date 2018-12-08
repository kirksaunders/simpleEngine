#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#define GLEW_STATIC

#include <string>
#include <vector>
#include <utility>

#include <GLEW/glew.h>

#include "render_base/texture.hpp"

namespace Render3D {
    class Window; // forward declaration

	class TextureManager {
	 public:
		static const unsigned int MAX_MATERIAL_TEXTURES = 5;

		TextureManager();

		void addWindow(const Window& win);
		void removeWindow(const Window& win);

		int getTextureLocation(GLuint texID, const Window& win);

		int makeTextureActive(GLuint texID, const Window& win);
		int makeTextureInactive(GLuint texID, const Window& win);

		static const std::string& getDiffuseName(unsigned int num);
		static const std::string& getSpecularName(unsigned int num);

		Texture& getDefaultTexture();
	 private:
	 	static const std::string diffNames[MAX_MATERIAL_TEXTURES];
		static const std::string specNames[MAX_MATERIAL_TEXTURES];

	 	Texture defaultTexture;
		std::vector<std::pair<const Window*, std::vector<GLuint> > > activeTexturesByWindow;
	};
}

#endif
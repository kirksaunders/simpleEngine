#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GLEW_STATIC

#include <string>
#include <unordered_map>

#include <GLEW/glew.h>

#include "render_base/shader.hpp"
#include "render_base/texturebuffer.hpp"
#include "render_base/window.hpp"

namespace Render3D {
	class Texture {
	 public:
		Texture(int w = 0, int h = 0);
		Texture(char const *filePath);
        Texture(const TextureBuffer& buff);
		
		void loadFromArray(int w, int h, GLubyte* data);
		void loadFromFile(char const *filePath);

		int getWidth();
		int getHeight();

		void setName(char const* name);
		char const* getName();

		void setShader(char const *name);
		char const* getShader();

		void use(Shader& shader);
		void use(Shader& shader, const char* name);
		int getLocation();
		void render(Shader& shader);

	 private:
		char const *textureName;
		int width;
		int height;
		GLuint textureID;
		char const *shaderName;
		GLuint VBO;
		GLuint TBO;

		static std::unordered_map<Window*, GLuint> VAOs;

		int useNextLocation();

		void generateBuffers();
		GLuint getVertexArrayObject(Window* win);
	};
}

#endif
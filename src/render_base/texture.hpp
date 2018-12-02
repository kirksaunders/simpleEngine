#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GLEW_STATIC

#include <string>
#include <unordered_map>

#include <GLEW/glew.h>

namespace Render3D {
    // forward declarations
    class Window;
    class Shader;
    class TextureManager;
    class TextureBuffer;

	class Texture {
	 public:
		Texture(int w = 0, int h = 0);
		Texture(const char *filePath);
        Texture(const TextureBuffer& buff);
        Texture(const Texture& other);
        Texture(Texture&& other);
        ~Texture();

        Texture& operator=(const Texture& other);
        Texture& operator=(Texture&& other);
		
		/*void loadFromArray(int w, int h, GLubyte* data);
		void loadFromFile(const char *filePath);*/

		int getWidth();
		int getHeight();

		void setName(const std::string& name);
		const char* getName();

		void setShader(Shader* s);
		Shader* const getShader();

		void use(Shader* shader, Window* win, TextureManager* textureManager);
		void use(Shader* shader, Window* win, TextureManager* textureManager, const char* name);
		void render(Shader* const shader, Window* const win, TextureManager* const textureManager);
        void prepareContent(Window* win, TextureManager* textureManager);

	 private:
        GLubyte* image;
		std::string textureName;
		int width;
		int height;
        int depth;
		Shader* shader;

        typedef std::pair<GLuint, GLuint> BufferPair;
        std::unordered_map<GLuint, BufferPair> bufferObjects;
		std::unordered_map<Window*, GLuint> VAOs;
        std::unordered_map<GLuint, GLuint> IDs;

		int getLocation(GLuint textureID, Window* win, TextureManager* textureManager);
		int useNextLocation(GLuint textureID, Window* win, TextureManager* textureManager);

		void generateBuffers(GLuint clusterID);
		void generateTexture(GLuint clusterID);
        void generateVertexArrayObject(GLuint clusterID, Window* win);
		GLuint getTextureID(GLuint clusterID);
		GLuint getVertexArrayObject(Window* win);

        void copy(const Texture& other);
        void move(Texture& other);
        void initialize();
        void destroy();
	};
}

#endif
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <utility>
#include <vector>

#include <glew.h>

#include "objects/bufferobject.hpp"

#include "render_base/shadervariable.hpp"

namespace Render3D {
    // forward declarations
    class Shader;
    class TextureBuffer;
    class TextureManager;
    class Window;

    struct TextureID {
        GLuint id;
        GLuint useCount;

        TextureID() : id(), useCount(0) {}
        TextureID(GLuint i) : id(i), useCount(0) {}
    };

    class Texture {
     public:
        Texture(int w = 0, int h = 0);
        Texture(const char *filePath);
        Texture(const TextureBuffer& buff);
        Texture(const Texture& other);
        Texture(Texture&& other) noexcept; // must be noexcept for vector to use it
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

        void use(Window& win, TextureManager& textureManager);
        void useDiffuse(Shader& shader, Window& win, TextureManager& textureManager, unsigned int num);
        void useSpecular(Shader& shader, Window& win, TextureManager& textureManager, unsigned int num);
        void resetDiffAndSpec(Shader& shader, Window& win, TextureManager& textureManager);
        void render(Window& win, TextureManager& textureManager);
        void prepareContent(Window& win, TextureManager& textureManager);
        void destroyContent(Window& win, TextureManager& textureManager);

     private:
        GLubyte* image;
        std::string textureName;
        int width;
        int height;
        int depth;
        Shader* shader;
        ShaderVariable<int>* textureVariable;

        std::vector<std::pair<GLuint, BufferObject> > bufferObjects;
        std::vector<std::pair<const Window*, GLuint> > VAOs;
        std::vector<std::pair<GLuint, TextureID> > IDs;

        int useNextLocation(GLuint textureID, Window& win, TextureManager& textureManager);

        BufferObject generateBuffers(GLuint clusterID);
        void destroyBuffers(GLuint clusterID);
        void generateTexture(GLuint clusterID);
        void destroyTexture(GLuint clusterID);
        void generateVertexArrayObject(Window& win);
        void destroyVertexArrayObject(Window& win);
        GLuint getTextureID(GLuint clusterID);
        GLuint getVertexArrayObject(Window& win);

        void copy(const Texture& other);
        void move(Texture& other);
        void initialize();
        void destroy();
    };
}

#endif
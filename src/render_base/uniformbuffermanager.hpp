#ifndef UNIFORMBUFFERMANAGER_HPP
#define UNIFORMBUFFERMANAGER_HPP

#include <vector>

#include <GLEW/glew.h>

namespace Render3D {
    class Window; // forward declaration

    class UniformBufferManager {
    public:
        int getBufferLocation(GLuint bufferID);
        int makeBufferActive(GLuint bufferID);
        int makeBufferInactive(GLuint bufferID);
    private:
        std::vector<GLuint> activeBuffers;
    };
};

#endif
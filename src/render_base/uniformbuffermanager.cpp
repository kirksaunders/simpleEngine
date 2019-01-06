#include "render_base/uniformbuffermanager.hpp"

using namespace Render3D;

int UniformBufferManager::getBufferLocation(GLuint bufferID) {
    for (unsigned int i = 0; i < activeBuffers.size(); ++i) {
        if (activeBuffers[i] == bufferID) {
            return i;
        }
    }

    return -1;
}

int UniformBufferManager::makeBufferActive(GLuint bufferID) {
    for (unsigned int i = 0; i < activeBuffers.size(); ++i) {
        if (activeBuffers[i] == 0) {
            activeBuffers[i] = bufferID;
            return i;
        }
    }

    activeBuffers.push_back(bufferID);

    return activeBuffers.size() - 1;
}

int UniformBufferManager::makeBufferInactive(GLuint bufferID) {
    for (unsigned int i = 0; i < activeBuffers.size(); ++i) {
        if (activeBuffers[i] == bufferID) {
            activeBuffers[i] = 0;
            return i;
        }
    }

    return -1;
}

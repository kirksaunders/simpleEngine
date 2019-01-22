#include <cstring>

#include "render_base/shadervariableblock.hpp"
#include "render_base/uniformbuffermanager.hpp"
#include "render_base/window.hpp"

using namespace Render3D;
using namespace Math3D;

static unsigned int GLSL_TYPE_BASE_ALIGNMENTS[] = {
    4,  // BOOL
    4,  // INT
    4,  // UINT
    4,  // FLOAT
    16, // VEC4
    16  // MAT4
};

static unsigned int GLSL_TYPE_SIZES[] = {
    4,  // BOOL
    4,  // INT
    4,  // UINT
    4,  // FLOAT
    16, // VEC4
    64  // MAT4
};

static const unsigned int TOTAL_SIZE_ALIGNMENT = 16;

// ShaderBlockVariable

void ShaderBlockVariableInterface::writeToArray(uint8_t* array) const {
    memcpy(array + offset, getDataPointer(), size);
}

void ShaderBlockVariableInterface::writeToBuffer(GLuint bufferID) {
    if (needsUpdated) {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, getDataPointer());
        needsUpdated = false;
    }
}

template <>
const void* ShaderBlockVariable<bool>::getDataPointer() const {
    return &value;
}

template <>
const void* ShaderBlockVariable<int>::getDataPointer() const {
    return &value;
}

template <>
const void* ShaderBlockVariable<unsigned int>::getDataPointer() const {
    return &value;
}

template <>
const void* ShaderBlockVariable<float>::getDataPointer() const {
    return &value;
}

template <>
const void* ShaderBlockVariable<Matrix4x4>::getDataPointer() const {
    return value.getValues();
}

template <>
const void* ShaderBlockVariable<Vector4>::getDataPointer() const {
    return value.getValues();
}

template <>
const void* ShaderBlockVariable<Color>::getDataPointer() const {
    return value.getValues();
}

// ShaderVariableBlock

ShaderVariableBlock::ShaderVariableBlock(std::initializer_list<GLSL_TYPE> types) {
    variables.reserve(types.size());

    size = 0;

    for (const GLSL_TYPE* it = types.begin(); it != types.end(); ++it) {
        unsigned int typeIndex = static_cast<int>(*it);
        unsigned int typeAlignment = GLSL_TYPE_BASE_ALIGNMENTS[typeIndex];
        unsigned int typeSize = GLSL_TYPE_SIZES[typeIndex];

        // align the data type (std140)
        if (size % typeAlignment > 0) {
            size += typeAlignment - size % typeAlignment;
        }

        ShaderBlockVariableInterface* rawPtr;

        switch (*it) {
         case GLSL_TYPE::BOOL:
            rawPtr = new ShaderBlockVariable<bool>(*this, size, typeSize);
            break;
         case GLSL_TYPE::INT:
            rawPtr = new ShaderBlockVariable<int>(*this, size, typeSize);
            break;
         case GLSL_TYPE::UINT:
            rawPtr = new ShaderBlockVariable<unsigned int>(*this, size, typeSize);
            break;
         case GLSL_TYPE::FLOAT:
            rawPtr = new ShaderBlockVariable<float>(*this, size, typeSize);
            break;
         case GLSL_TYPE::VEC4:
            rawPtr = new ShaderBlockVariable<Vector4>(*this, size, typeSize);
            break;
         case GLSL_TYPE::MAT4:
            rawPtr = new ShaderBlockVariable<Matrix4x4>(*this, size, typeSize);
            break;
         default:
            throw Exception("Unable to create shader variable block, unknown data type");
        }

        variables.push_back(std::unique_ptr<ShaderBlockVariableInterface>(std::move(rawPtr)));

        size += typeSize;
    }

    // Debugging with RenderDoc produces a warning about the provided buffer not being big enough, and I've
    // noticed that it wants data with a size that is a multiple 16 bytes, so that's what I'm doing here.
    // I could NOT find any documentation or mention of this in the opengl standard, so this could be an issue
    // with RenderDoc or some sort of driver-related quirk.
    if (size % TOTAL_SIZE_ALIGNMENT > 0) {
        size += TOTAL_SIZE_ALIGNMENT - size % TOTAL_SIZE_ALIGNMENT;
    }
}

int ShaderVariableBlock::use(Window& win, UniformBufferManager& uniformBufferManager) {
    GLuint clusterID = win.getClusterID();
    GLuint bufferID = getBufferID(clusterID);

    int location = uniformBufferManager.getBufferLocation(bufferID);
    if (location == -1) {
        location = useNextLocation(bufferID, win, uniformBufferManager);
    }

    return location;
}

int ShaderVariableBlock::useNextLocation(GLuint bufferID, Window& win, UniformBufferManager& uniformBufferManager) {
    int location = uniformBufferManager.makeBufferActive(bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    glBindBufferBase(GL_UNIFORM_BUFFER, location, bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return location;
}

void ShaderVariableBlock::generateBuffer(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            ++(bufferObjects[i].second.useCount);
            return;
        }
    }

    uint8_t* data = new uint8_t[size];

    for (unsigned int i = 0; i < variables.size(); ++i) {
        variables[i]->writeToArray(data);
    }

    // Generate GL Buffer
    BufferObject buffer;

    glGenBuffers(1, &buffer.id);

    glBindBuffer(GL_UNIFORM_BUFFER, buffer.id);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW); // using dynamic because this data is likely to change
    delete[] data;
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    buffer.useCount = 1;
    bufferObjects.push_back(std::pair<GLuint, BufferObject>(clusterID, std::move(buffer)));
}

void ShaderVariableBlock::destroyBuffer(GLuint clusterID) {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            if (--(bufferObjects[i].second.useCount) == 0) {
                glDeleteBuffers(1, &bufferObjects[i].second.id);
                std::swap(bufferObjects[i], bufferObjects.back());
                bufferObjects.pop_back();
            }
            return;
        }
    }
}

GLuint ShaderVariableBlock::getBufferID(GLuint clusterID) const {
    for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
        if (bufferObjects[i].first == clusterID) {
            return bufferObjects[i].second.id;
        }
    }

    throw Exception("Unable to get shader variable block buffer for given window, doesn't exist");
}

void ShaderVariableBlock::prepareContent(Window& win) {
    generateBuffer(win.getClusterID());
}

void ShaderVariableBlock::updateContent(Window& win) {
    GLuint bufferID = getBufferID(win.getClusterID());

    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);

    for (unsigned int i = 0; i < variables.size(); ++i) {
        variables[i]->writeToBuffer(bufferID);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderVariableBlock::destroyContent(Window& win) {
    destroyBuffer(win.getClusterID());
}

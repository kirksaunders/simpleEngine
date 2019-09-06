#include "objects/primitive3d.hpp"

#include "render_base/shader.hpp"
#include "render_base/window.hpp"

#include "utilities/exception.hpp"

using namespace Render3D;
using namespace Math3D;

void Primitive3D::setSize(const Vector4& value) {
    size = value;
}

Vector4 Primitive3D::getSize() const {
    return size;
}

void Primitive3D::setCFrame(const Matrix4x4& value) {
    cframe = value;
}

Matrix4x4 Primitive3D::getCFrame() const {
    return cframe;
}

void Primitive3D::setColor(const Color& value) {
    color = value;
}

Color Primitive3D::getColor() const {
    return color;
}

void Primitive3D::setShader(std::shared_ptr<Shader> s) {
    shader = std::move(s);
    if (shader != nullptr) {
        modelCFrameVariable = shader->getVariable<Matrix4x4>("modelCFrame");
        modelRotationVariable = shader->getVariable<Matrix4x4>("modelRotation");
        modelSizeVariable = shader->getVariable<Vector4>("modelSize");
        modelColorVariable = shader->getVariable<Color>("modelColor");
    } else {
        modelCFrameVariable = nullptr;
        modelRotationVariable = nullptr;
        modelSizeVariable = nullptr;
        modelColorVariable = nullptr;
    }
}

std::shared_ptr<Shader> Primitive3D::getShader() const {
    return shader;
}

Instance* Primitive3D::newInstance(const Matrix4x4& cfr) {
    Instance* newInstance = new Instance(cfr);
    instances.push_back(newInstance);

    return newInstance;
}

void Primitive3D::deleteInstance(Instance*& instance) {
    for (unsigned int i = 0; i < instances.size(); ++i) {
        if (instances[i] == instance) {
            instances.erase(instances.begin() + i);
            delete instance;
            instance = nullptr;
            return;
        }
    }

    throw Exception("Unable to delete instance from object, instance not found");
}

void Primitive3D::clearInstances() {
    for (unsigned int i = 0; i < instances.size(); ++i) {
        delete instances[i];
    }

    instances.clear();
}

void Primitive3D::applyVariables(Window& win) {
    Matrix4x4 rotation = cframe.rotation();

    modelCFrameVariable->setValue(win, cframe);
    modelRotationVariable->setValue(win, rotation);
    modelSizeVariable->setValue(win, size);
    modelColorVariable->setValue(win, color);
}

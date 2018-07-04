#include "objects/primitive3d.hpp"

using namespace Render3D;
using namespace Math3D;

Primitive3D::Primitive3D() {
    shaderName = "defaultPerspective"; // default shader used to render 3d objects
}

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

void Primitive3D::setShader(char const *name) {
    shaderName = name;
}

char const* Primitive3D::getShader() const {
    return shaderName;
}

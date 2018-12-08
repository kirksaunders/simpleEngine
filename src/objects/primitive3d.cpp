#include "objects/primitive3d.hpp"

#include "render_base/shader.hpp"

using namespace Render3D;
using namespace Math3D;

Primitive3D::Primitive3D() {
	shader = nullptr;
    modelCFrameVariable = nullptr;
    modelRotationVariable = nullptr;
    modelSizeVariable = nullptr;
    modelColorVariable = nullptr;
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

void Primitive3D::setShader(Shader* const s) {
	shader = s;
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

Shader* const Primitive3D::getShader() const {
	return shader;
}

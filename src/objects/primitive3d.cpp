#include "objects/primitive3d.hpp"

#include "render_base/shader.hpp"
#include "render_base/window.hpp"

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

void Primitive3D::setShader(Shader* const s) {
	shader = s;
    if (shader != nullptr) {
        modelCFrameVariable = shader->getVariable<Matrix4x4>("modelCFrame");
        modelRotationVariable = shader->getVariable<Matrix4x4>("modelRotation");
        modelSizeVariable = shader->getVariable<Vector4>("modelSize");
        modelColorVariable = shader->getVariable<Color>("modelColor");
        wireframeVariable = shader->getVariable<int>("wireframeEnabled");
    } else {
        modelCFrameVariable = nullptr;
        modelRotationVariable = nullptr;
        modelSizeVariable = nullptr;
        modelColorVariable = nullptr;
		wireframeVariable = nullptr;
    }
}

Shader* const Primitive3D::getShader() const {
	return shader;
}

void Primitive3D::setWireframeEnabled(bool enabled) {
	wireframeEnabled = enabled;
}

bool Primitive3D::isWireframeEnabled() const {
	return wireframeEnabled;
}

void Primitive3D::applyVariables(Window& win) {
	Matrix4x4 rotation = cframe.rotation();

	modelCFrameVariable->setValue(win, cframe);
	modelRotationVariable->setValue(win, rotation);
	modelSizeVariable->setValue(win, size);
	modelColorVariable->setValue(win, color);
	wireframeVariable->setValue(win, static_cast<int>(wireframeEnabled));
}

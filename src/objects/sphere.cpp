#include "objects/sphere.hpp"

using namespace Render3D;
using namespace Math3D;

Sphere::Sphere() {
	shaderName = "defaultPerspective"; // default shader used to render cuboids
	model = Model("res/meshes/sphere.obj");
}

void Sphere::setSize(const Vector4& value) {
	size = value;
	model.setSize(value);
}

Vector4 Sphere::getSize() const {
	return size;
}

void Sphere::setCFrame(const Matrix4x4& value) {
	cframe = value;
	model.setCFrame(value);
}

Matrix4x4 Sphere::getCFrame() const {
	return cframe;
}

void Sphere::setColor(const Color& value) {
	color = value;
	model.setColor(value);
}

Color Sphere::getColor() const {
	return color;
}

void Sphere::setShader(char const *name) {
	shaderName = name;
	model.setShader(name);
}

char const* Sphere::getShader() const {
	return shaderName;
}

void Sphere::render(Shader& shader) {
	model.render(shader);
}
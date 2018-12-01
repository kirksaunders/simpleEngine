#include "objects/sphere.hpp"

using namespace Render3D;
using namespace Math3D;

Sphere::Sphere() : Primitive3D() {
	model = Model("res/meshes/sphere.obj");
}

void Sphere::setSize(const Vector4& value) {
	size = value;
	model.setSize(value);
}

void Sphere::setCFrame(const Matrix4x4& value) {
	cframe = value;
	model.setCFrame(value);
}

void Sphere::setColor(const Color& value) {
	color = value;
	model.setColor(value);
}

void Sphere::render(Shader* const shader, Window* const win, TextureManager* const textureManager) {
	model.render(shader, win, textureManager);
}

void Sphere::prepareContent(Window* win, TextureManager* textureManager) {
    model.prepareContent(win, textureManager);
}

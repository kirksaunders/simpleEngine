#include <cstring>
#include <iostream>
#include <utility>

#include "objects/primitive3d.hpp"

#include "render_base/exception.hpp"
#include "render_base/context3d.hpp"
#include "render_base/window.hpp"
#include "render_base/texture.hpp"
#include "render_base/texturebuffer.hpp"
#include "render_base/shader.hpp"

using namespace Render3D;
using namespace Math3D;

Context3D::Context3D() {
	window = nullptr;
}

Context3D::Context3D(Window* win) {
	window = win;
    window->context = this;
    window->makeCurrent();

    if (window->isChild()) {
        std::cout << "Using existing manager of window " << window->parentWindow << " for child window " << win << std::endl;
        // use existing shader and texture managers
        textureManager = window->parentWindow->context->textureManager;
        std::cout << "Used manager successfully, use_count is " << textureManager.use_count() << std::endl;
    } else {
        std::cout << "Creating manager for window " << win << std::endl;
        // create shader and texture managers
        textureManager = std::make_shared<TextureManager>();
        std::cout << "Created manager successfully, use_count is " << textureManager.use_count() << std::endl;
    }

    textureManager->addWindow(*window);
	textureManager->getDefaultTexture().prepareContent(*window, *textureManager.get());
}

Context3D::~Context3D() {
	if (textureManager.use_count() == 1) {
		textureManager->getDefaultTexture().destroyContent(*window, *textureManager.get());
	}
    textureManager->removeWindow(*window);
}

void Context3D::addObject(Primitive3D* object) {
	std::pair<std::set<Primitive3D*>::iterator, bool> p = objects.insert(object);
    if (p.second) {
		window->makeCurrent();
		object->prepareContent(*window, *textureManager.get());
	}
}

void Context3D::removeObject(Primitive3D* object) {
	window->makeCurrent();
	unsigned int numRemoved = objects.erase(object);
    if (numRemoved > 0) {
		object->destroyContent(*window, *textureManager.get());
	}
}

void Context3D::clearObjects() {
	window->makeCurrent();
	for (std::set<Primitive3D*>::iterator it = objects.begin(); it != objects.end(); it++) {
		(*it)->destroyContent(*window, *textureManager.get());
	}
	objects.clear();
}

void Context3D::addShader(Shader* shader) {
	std::pair<std::set<Shader*>::iterator, bool> p = shaders.insert(shader);
    if (p.second) {
		window->makeCurrent();
		shader->prepareContent(*window);
	}
}

void Context3D::removeShader(Shader* shader) {
	unsigned int numRemoved = shaders.erase(shader);
    if (numRemoved > 0) {
	    window->makeCurrent();
		shader->destroyContent(*window);
	}
}

void Context3D::clearShaders() {
	window->makeCurrent();
	for (std::set<Shader*>::iterator it = shaders.begin(); it != shaders.end(); it++) {
		(*it)->destroyContent(*window);
	}
	shaders.clear();
}

void Context3D::addTexture(Texture* texture) {
	std::pair<std::set<Texture*>::iterator, bool> p = textures.insert(texture);
    if (p.second) {
		window->makeCurrent();
		texture->prepareContent(*window, *textureManager.get());
	}
}

void Context3D::removeTexture(Texture* texture) {
	unsigned int numRemoved = textures.erase(texture);
    if (numRemoved > 0) {
	    window->makeCurrent();
		texture->destroyContent(*window, *textureManager.get());
	}
}

void Context3D::clearTextures() {
	window->makeCurrent();
	for (std::set<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
		(*it)->destroyContent(*window, *textureManager.get());
	}
	textures.clear();
}

Camera* const Context3D::getCamera() {
	return &camera;
}

TextureManager* const Context3D::getTextureManager() {
	return textureManager.get();
}

void Context3D::render() {
	window->makeCurrent();
	float aspectRatio = (float) window->getWidth() / window->getHeight();

	Matrix4x4 cameraCFrame = camera.getCFrame();
	Vector4 cameraPosition = cameraCFrame.position();
	Matrix4x4 cameraInverse = cameraCFrame.inverse();
	Matrix4x4 projection = camera.getPerspective(aspectRatio);

	Vector4 lightPosition = Vector4(40, 300, 150);
	Color lightColor = Color(1, 1, 1);
	Color ambient = Color(0.15, 0.15, 0.15);

	Shader* currentShader = nullptr;

	Texture& defaultTex = textureManager->getDefaultTexture();
 
	for (std::set<Primitive3D*>::iterator it = objects.begin(); it != objects.end(); it++) {
		Primitive3D* object = *it;

		if (object->getShader() == nullptr) {
			throw Exception("Tried to render object with no shader set");
		}

		if (object->getShader() != currentShader) {
			currentShader = object->getShader();
			currentShader->use(*window);

			defaultTex.resetDiffAndSpec(*currentShader, *window, *textureManager.get()); // reset textures

			// Scene Lighting Data
            currentShader->getVariable<Vector4>("lightPos")->setValue(*window, lightPosition);
            currentShader->getVariable<Color>("lightColor")->setValue(*window, lightColor);
            currentShader->getVariable<Color>("ambientColor")->setValue(*window, ambient);

			// Camera Data
            currentShader->getVariable<Vector4>("cameraPos")->setValue(*window, cameraPosition);
            currentShader->getVariable<Matrix4x4>("cameraInverse")->setValue(*window, cameraInverse);

			// Projection Matrix
            currentShader->getVariable<Matrix4x4>("projection")->setValue(*window, projection);
		}
		object->render(*window, *textureManager.get());
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void Context3D::renderTexture(Texture& tex) {
	window->makeCurrent();
	if (tex.getShader() == nullptr) {
		throw Exception("Tried to render texture with no shader set");
	}
	tex.getShader()->use(*window);
	tex.render(*window, *textureManager.get());
	glBindVertexArray(0);
	glUseProgram(0);
}

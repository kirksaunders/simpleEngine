#include <cstring>
#include <iostream>
#include <utility>

#include "objects/primitive3d.hpp"

#include "render_base/context3d.hpp"
#include "render_base/window.hpp"
#include "render_base/texture.hpp"
#include "render_base/texturebuffer.hpp"
#include "render_base/shader.hpp"

using namespace Render3D;
using namespace Math3D;

Context3D::Context3D() {
	window = NULL;
}

Context3D::Context3D(Window* win) {
	window = win;
    window->context = this;
    window->makeCurrent();

    if (window->isChild()) {
        std::cout << "Using existing managers of window " << window->parentWindow << " for child window " << win << std::endl;
        // use existing shader and texture managers
        shaderManager = window->parentWindow->context->shaderManager;
        textureManager = window->parentWindow->context->textureManager;
        std::cout << "Used managers successfully, use_count is " << shaderManager.use_count() << std::endl;
    } else {
        std::cout << "Creating managers for window " << win << std::endl;
        // create shader and texture managers
        shaderManager = std::make_shared<ShaderManager>();
        textureManager = std::make_shared<TextureManager>();
        std::cout << "Created managers successfully, use_count is " << shaderManager.use_count() << std::endl;
    }

	if (!shaderManager->shaderDoesExist("defaultPerspective")) {
		Shader perspective("res/defaultPerspective.vert", "res/defaultPerspective.frag");
        perspective.prepareContent(window);
		shaderManager->addShader("defaultPerspective", perspective);
	} else {
        shaderManager->getShader("defaultPerspective")->prepareContent(window);
    }

	if (!shaderManager->shaderDoesExist("imageRender")) {
		Shader imageRender("res/imageRender.vert", "res/imageRender.frag");
        imageRender.prepareContent(window);
		shaderManager->addShader("imageRender", imageRender);
	} else {
        shaderManager->getShader("imageRender")->prepareContent(window);
    }

	if (!textureManager->textureDoesExist("defaultTexture")) {
		TextureBuffer buff(1, 1, 4);
		buff.setPixel(0, 0, Color(0, 0, 0, 0));

		Texture defaultTexture(buff);
        defaultTexture.prepareContent(win, textureManager.get());
		textureManager->addTexture("defaultTexture", std::move(defaultTexture));
	} else {
        textureManager->getTexture("defaultTexture")->prepareContent(win, textureManager.get());
    }
    textureManager->addWindow(window);
}

Context3D::~Context3D() {
    textureManager->removeWindow(window);
}

void Context3D::addObject(Primitive3D* object) {
	objects.push_back(object);
    object->prepareContent(window, textureManager.get());
}

Camera* Context3D::getCamera() {
	return &camera;
}

TextureManager* Context3D::getTextureManager() {
	return textureManager.get();
}

void Context3D::render() {
	//window->makeCurrent();
	float aspectRatio = (float) window->getWidth() / window->getHeight();

	Matrix4x4 cameraCFrame = camera.getCFrame();
	Vector4 cameraPosition = cameraCFrame.position();
	Matrix4x4 cameraInverse = cameraCFrame.inverse();
	Matrix4x4 projection = camera.getPerspective(aspectRatio);

	Vector4 lightPosition = Vector4(40, 300, 150);
	Color lightColor = Color(1, 1, 1);
	Color ambient = Color(0.15, 0.15, 0.15);

	std::string currentShader = "";
	Shader* shaderObject;

	Texture* defaultTex = textureManager->getTexture("defaultTexture");
 
	for (int i = 0; i < objects.size(); i++) {
		Primitive3D* object = objects[i];

		std::string shader = object->getShader();
		if (currentShader != shader) {
			shaderObject = shaderManager->getShader(shader);
			shaderObject->use(window);
			currentShader = shader;

			for (int i2 = 0; i2 < 5; i2++) { // reset textures
				std::string name = "texture_diffuse";
				name += std::to_string(i2 + 1);
				defaultTex->use(shaderObject, window, textureManager.get(), name.c_str());
				name = "texture_specular";
				name += std::to_string(i2 + 1);
				defaultTex->use(shaderObject, window, textureManager.get(), name.c_str());
			}

			// Scene Lighting Data
			shaderObject->setVariable(window, "lightPos", lightPosition);
			shaderObject->setVariable(window, "lightColor", lightColor);
			shaderObject->setVariable(window, "ambientColor", ambient);

			// Camera Data
			shaderObject->setVariable(window, "cameraPos", cameraPosition);
			shaderObject->setVariable(window, "cameraInverse", cameraInverse);

			// Projection Matrix
			shaderObject->setVariable(window, "projection", projection);
		}
		object->render(shaderObject, window, textureManager.get());
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void Context3D::renderTexture(Texture& tex) {
	//window->makeCurrent();
    tex.prepareContent(window, textureManager.get());
	const char* shader = tex.getShader();
	Shader* shaderObject = shaderManager->getShader(shader);
	shaderObject->use(window);
	tex.render(shaderObject, window, textureManager.get());
	glUseProgram(0);
}

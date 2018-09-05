#include "render_base/context3d.hpp"

using namespace Render3D;
using namespace Math3D;

Context3D::Context3D() {
	window = NULL;
}

Context3D::Context3D(Window* win) {
	window = win;

	if (!shaderManager.shaderDoesExist("defaultPerspective")) {
		Shader perspective("res/defaultPerspective.vert", "res/defaultPerspective.frag");
		shaderManager.addShader("defaultPerspective", perspective);
	}

	if (!shaderManager.shaderDoesExist("imageRender")) {
		Shader imageRender("res/imageRender.vert", "res/imageRender.frag");
		shaderManager.addShader("imageRender", imageRender);
	}

	if (!textureManager.textureDoesExist("defaultTexture")) {
		TextureBuffer buff(1, 1, 4);
		buff.setPixel(0, 0, Color(0, 0, 0, 0));

		Texture defaultTexture(1, 1);
		defaultTexture.loadFromArray(1, 1, buff.getAddress());
		textureManager.addTexture("defaultTexture", defaultTexture);
	}

	textureManager.addWindow(win);
}

void Context3D::addObject(Primitive3D* object) {
	objects.push_back(object);
}

Camera* Context3D::getCamera() {
	return &camera;
}

void Context3D::render() {
	window->activate();
	float aspectRatio = (float) window->getWidth() / window->getHeight();

	Matrix4x4 cameraCFrame = camera.getCFrame();
	Matrix4x4 cameraPosition = cameraCFrame.position();
	Matrix4x4 cameraInverse = cameraCFrame.inverse();
	Matrix4x4 projection = camera.getPerspective(aspectRatio);

	Vector4 lightPosition = Vector4(40, 300, 150);
	Color lightColor = Color(1, 1, 1);
	Color ambient = Color(0.15, 0.15, 0.15);

	char const* currentShader = "";
	Shader* shaderObject;

	Texture* defaultTex = textureManager.getTexture("defaultTexture");
 
	for (int i = 0; i < objects.size(); i++) {
		Primitive3D* object = objects[i];

		char const* shader = object->getShader();
		if (strcmp(currentShader, shader) != 0) {
			shaderObject = shaderManager.getShader(shader);
			shaderObject->use();
			currentShader = shader;

			for (int i2 = 0; i2 < 5; i2++) { // reset textures
				std::string name = "texture_diffuse";
				name += std::to_string(i2 + 1);
				defaultTex->use(*shaderObject, name.c_str());
				name = "texture_specular";
				name += std::to_string(i2 + 1);
				defaultTex->use(*shaderObject, name.c_str());
			}

			// Scene Lighting Data
			shaderObject->setVariable("lightPos", lightPosition);
			shaderObject->setVariable("lightColor", lightColor);
			shaderObject->setVariable("ambientColor", ambient);

			// Camera Data
			shaderObject->setVariable("cameraPos", cameraPosition);
			shaderObject->setVariable("cameraInverse", cameraInverse);

			// Projection Matrix
			shaderObject->setVariable("projection", projection);
		}
		object->render(*shaderObject);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void Context3D::renderTexture(Texture& tex) {
	window->activate();
	char const* shader = tex.getShader();
	Shader* shaderObject = shaderManager.getShader(shader);
	shaderObject->use();
	tex.render(*shaderObject);
	glUseProgram(0);
}

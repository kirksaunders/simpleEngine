#include "render_base/context3d.hpp"

using namespace Render3D;
using namespace Math3D;

bool sortTriangles(Triangle& a, Triangle& b) {
	return (a.getDistanceCache() < b.getDistanceCache());
}

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

void Context3D::addObject(Triangle* triangle) {
	triangles.push_back(triangle);
}

void Context3D::addObject(Face* face) {
	faces.push_back(face);
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

	// Below is legacy rendering code. Might make this support legacy sometime for older computers(school computers).

	/*std::vector<Triangle> tris = getTriangles();

	sort(tris.begin(), tris.end());

	for (int i = 0; i < tris.size(); ++i) {
		Triangle t = tris[i];

		Vector4 v1 = t.getVertex1();
		v1 = cameraInverse * v1;
		v1 = projection * v1;
		v1 = v1 / v1[3];

		Vector4 v2 = t.getVertex2();
		v2 = cameraInverse * v2;
		v2 = projection * v2;
		v2 = v2 / v2[3];

		Vector4 v3 = t.getVertex3();
		v3 = cameraInverse * v3;
		v3 = projection * v3;
		v3 = v3 / v3[3];

		Vector4 normal = t.getNormal();

		Color color = t.getColor();

		float dot = lightDirection.dot(normal);

		dot = (dot + 1) * 0.5;

		color = color * dot;

		//window->drawTriangle(v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], color);
	}
	tris.clear();*/
}

void Context3D::renderTexture(Texture& tex) {
	window->activate();
	char const* shader = tex.getShader();
	Shader* shaderObject = shaderManager.getShader(shader);
	shaderObject->use();
	tex.render(*shaderObject);
	glUseProgram(0);
}

std::vector<Triangle> Context3D::getTriangles() { // legacy code
	/*std::vector<Triangle> tris;

	for (int i = 0; i < cuboids.size(); ++i) {
		std::vector<Face> f = cuboids[i]->getFaces();

		for (int i2 = 0; i2 < f.size(); ++i2) {
			std::vector<Triangle> t = f[i2].getTriangles();

			for (int i3 = 0; i3 < t.size(); ++i3) {
				Triangle tri = t[i3];
				Vector4 dis = tri.getCenter() - camera.getCFrame().position();
				tri.setDistanceCache(dis.dot(dis));
				tris.push_back(tri);
			}
		}
	}

	for (int i = 0; i < faces.size(); ++i) {
		std::vector<Triangle> t = faces[i]->getTriangles();

		for (int i2 = 0; i2 < t.size(); ++i2) {
			Triangle tri = t[i2];
			Vector4 dis = tri.getCenter() - camera.getCFrame().position();
			tri.setDistanceCache(dis.dot(dis));
			tris.push_back(tri);
		}
	}

	for (int i = 0; i < triangles.size(); ++i) {
		Triangle* tri = triangles[i];
		Vector4 dis = tri->getCenter() - camera.getCFrame().position();
		tri->setDistanceCache(dis.dot(dis));
		tris.push_back(Triangle(tri->getVertex1(), tri->getVertex2(), tri->getVertex3(), tri->getColor()));
	}

	return tris;*/
}

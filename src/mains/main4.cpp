#include "mains/mains.hpp"

#include "render_base/shadermanager.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main4() {
	std::string meshPath;
	std::cout << "Enter mesh file path: ";
	std::getline(std::cin, meshPath);

	float r, g, b;
	std::cout << "Enter color for mesh(rgb): ";
	std::cin >> r >> g >> b;

	float sx, sy, sz;
	std::cout << "Enter size for mesh(xyz): ";
	std::cin >> sx >> sy >> sz;

	Window* window1 = new Window(WIDTH, HEIGHT, "Testing1");
	Window* window2 = new Window(WIDTH, HEIGHT, "Testing2");

	Context3D context1 = Context3D(window1);
	Context3D context2 = Context3D(window2);

	Camera* cam1 = context1.getCamera();
	cam1->setCFrame(Matrix4x4(0, 4, 0) * Matrix4x4::fromEuler(-0.25, 0, 0));
	Camera* cam2 = context2.getCamera();
	cam2->setCFrame(Matrix4x4(0, 4, 0) * Matrix4x4::fromEuler(-0.25, 0, 0));

	Model* ground = new Model("res/meshes/ground.obj");
	ground->setSize(Vector4(1, 1, 1));
	ground->setCFrame(Matrix4x4(0, -1.5, 0));
	ground->setColor(Color(0, 1, 0));

	context1.addObject(ground);
	context2.addObject(ground);

	Model* model = new Model(meshPath.c_str());

	model->setSize(Vector4(sx, sy, sz));
	model->setCFrame(Matrix4x4(0, 3, -8) * Matrix4x4::fromEuler(0.75, 0.75, 0));
	model->setColor(Color(r, g, b));

	context1.addObject(model);
	context2.addObject(model);

	/*Shader testShader("res/test.vert", "res/test.frag");
	shaderManager.addShader("test", testShader);

	model->setShader("test");*/

	window1->setVSyncEnabled(true);
	window2->setVSyncEnabled(false);

	float x, y, z;
	x = y = z = 0;

	float cX1, cY1;
	cX1 = cY1 = 0;
	bool rightMouseDown1 = false;
	double mX1, mY1;
	window1->setMouseDownCallback([&window1, &rightMouseDown1, &mX1, &mY1](int button) {
		if (button == 1) { // right mouse button
			window1->setMouseLockEnabled(true);
			rightMouseDown1 = true;
			window1->getMousePosition(mX1, mY1);
		}
	});
	window1->setMouseUpCallback([&window1, &rightMouseDown1](int button) {
		if (button == 1) { // right mouse button
			window1->setMouseLockEnabled(false);
			rightMouseDown1 = false;
		}
	});
	window1->setMouseMoveCallback([&rightMouseDown1, &mX1, &mY1, &cX1, &cY1](double x, double y) {
		if (rightMouseDown1) {
			cX1 -= (x - mX1);
			cY1 -= (y - mY1);
			mX1 = x;
			mY1 = y;
		}
	});
	float cX2, cY2;
	cX2 = cY2 = 0;
	bool rightMouseDown2 = false;
	double mX2, mY2;
	window2->setMouseDownCallback([&window2, &rightMouseDown2, &mX2, &mY2](int button) {
		if (button == 1) { // right mouse button
			window2->setMouseLockEnabled(true);
			rightMouseDown2 = true;
			window2->getMousePosition(mX2, mY2);
		}
	});
	window2->setMouseUpCallback([&window2, &rightMouseDown2](int button) {
		if (button == 1) { // right mouse button
			window2->setMouseLockEnabled(false);
			rightMouseDown2 = false;
		}
	});
	window2->setMouseMoveCallback([&rightMouseDown2, &mX2, &mY2, &cX2, &cY2](double x, double y) {
		if (rightMouseDown2) {
			cX2 -= (x - mX2);
			cY2 -= (y - mY2);
			mX2 = x;
			mY2 = y;
		}
	});

	while (window1->isActive() || window2->isActive()) {
		x = x + 0.01;
		y = y + 0.01;
		z = z + 0.01;

		model->setCFrame(Matrix4x4(model->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

		if (window1->isActive()) {
			window1->clear();

			updateCamera(cam1, window1, cX1, cY1);
			context1.render();

			window1->update();
			window1->pollEvents();
		} else {
			window1->close();
		}
		if (window2->isActive()) {
			window2->clear();

			updateCamera(cam2, window2, cX2, cY2);
			context2.render();

			window2->update();
			window2->pollEvents();
		} else {
			window2->close();
		}
	}

	delete window1;
	delete window2;
	delete ground;
	delete model;
	glfwTerminate();
}

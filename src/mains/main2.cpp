#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main2() {
	std::string imagePath;
	std::cout << "Enter image file path: ";
	std::getline(std::cin, imagePath);

	Window* window1 = new Window(WIDTH, HEIGHT, "Testing1");
	Window* window2 = new Window(WIDTH, HEIGHT, "Testing2");

	Context3D context1 = Context3D(window1);
	Context3D context2 = Context3D(window2);

	Cuboid* cube = new Cuboid();
	cube->setSize(Vector4(1, 1, 1));
	cube->setCFrame(Matrix4x4(0, 0, -5));
	cube->setColor(Color(0.2, 0.2, 0.85));

	context1.addObject(cube);
	context2.addObject(cube);

	Texture tex(imagePath.c_str());

	window1->setVSyncEnabled(true);
	window2->setVSyncEnabled(true);

	while (window1->isActive() || window2->isActive()) {
		if (window1->isActive()) {
			window1->clear();

			context1.renderTexture(tex);

			window1->update();
			window1->pollEvents();
		} else {
			window1->close();
		}
		if (window2->isActive()) {
			window2->clear();

			context2.renderTexture(tex);

			window2->update();
			window2->pollEvents();
		} else {
			window2->close();
		}
	}

	delete window1;
	delete window2;
	glfwTerminate();
}

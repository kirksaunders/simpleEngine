#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main1() {
    std::string meshPath;
    std::cout << "Enter mesh file path: ";
    std::getline(std::cin, meshPath);

    float r, g, b;
    std::cout << "Enter color for mesh(rgb): ";
    std::cin >> r >> g >> b;

    float sx, sy, sz;
    std::cout << "Enter size for mesh(xyz): ";
    std::cin >> sx >> sy >> sz;

	Window* window = new Window(WIDTH, HEIGHT, "Main 1 - Model Loading");

	Context3D context = Context3D(window);

    Camera* cam = context.getCamera();
    cam->setCFrame(Matrix4x4(0, 4, 0));

    Model* ground = new Model("res/meshes/ground.obj");
    ground->setSize(Vector4(1, 1, 1));
    ground->setCFrame(Matrix4x4(0, -1.5, 0));
    ground->setColor(Color(0, 1, 0));

    context.addObject(ground);

	Model* model = new Model(meshPath.c_str());

	model->setSize(Vector4(sx, sy, sz));
	model->setCFrame(Matrix4x4(0, 3, -8) * Matrix4x4::fromEuler(0.75, 0.75, 0));
	model->setColor(Color(r, g, b));

	context.addObject(model);

	Model* model2 = new Model("res/meshes/blade_thinner.obj");

	model2->setSize(Vector4(0.75, 0.75, 0.75));
	model2->setCFrame(Matrix4x4(8, 3, 0) * Matrix4x4::fromEuler(0.75, 0.75, 0));
	model2->setColor(Color(0.2, 0.2, 0.2));

	context.addObject(model2);

	Cuboid* cube = new Cuboid();

	cube->setSize(Vector4(2, 1, 1));
	cube->setCFrame(Matrix4x4(-8, 3, 0));
	cube->setColor(Color(0.2, 0.2, 0.85));

	context.addObject(cube);

	Sphere* sphere = new Sphere();

	sphere->setSize(Vector4(2, 2, 2));
	sphere->setCFrame(Matrix4x4(0, 3, 8));
	sphere->setColor(Color(0.2, 0.2, 0.85));

	context.addObject(sphere);

	Cuboid* cube2 = new Cuboid();

	cube2->setSize(Vector4(0.25, 0.5, 0.25));
	cube2->setColor(Color(0.85, 0.2, 0.2));

	context.addObject(cube2);

    /*Shader testShader("res/test.vert", "res/test.frag");
    shaderManager.addShader("test", testShader);

    model->setShader("test");*/

	float x, y, z;
    x = y = z = 0;

    float cX, cY;
    cX = 0;
    cY = -100;
    bool rightMouseDown = false;
    double mX, mY;
    window->setMouseDownCallback([&window, &rightMouseDown, &mX, &mY](int button) {
        if (button == 1) { // right mouse button
            window->setMouseLockEnabled(true);
            rightMouseDown = true;
            window->getMousePosition(mX, mY);
        }
    });
    window->setMouseUpCallback([&window, &rightMouseDown](int button) {
        if (button == 1) { // right mouse button
            window->setMouseLockEnabled(false);
            rightMouseDown = false;
        }
    });
    window->setMouseMoveCallback([&rightMouseDown, &mX, &mY, &cX, &cY](double x, double y) {
        if (rightMouseDown) {
            cX -= (x - mX);
            cY -= (y - mY);
            mX = x;
            mY = y;
        }
    });

	window->setVSyncEnabled(true);

	while (window->isActive()) {
		window->clear();

		x = x + 0.01;
		y = y + 0.01;
		z = z + 0.01;

		cube->setCFrame(Matrix4x4(cube->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
		cube2->setCFrame(cube->getCFrame() * Matrix4x4(0, cube->getSize()[1] / 2 + cube2->getSize()[1] / 2, 0));
		model->setCFrame(Matrix4x4(model->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
		model2->setCFrame(Matrix4x4(model2->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
        sphere->setCFrame(Matrix4x4(sphere->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        updateCamera(cam, window, cX, cY);

		context.render();

		window->update();
		window->pollEvents();
	}

	delete window;
	delete cube;
	delete cube2;
	delete sphere;
    delete ground;
	delete model;
	delete model2;
    glfwTerminate();
}

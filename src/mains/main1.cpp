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

    Window window(WIDTH, HEIGHT, "Main 1 - Model Loading");

    Context3D* context = window.getContext();

    std::shared_ptr<Shader> defaultShader = Shader::defaultPerspective();
    context->addShader(defaultShader);

    Camera* cam = context->getCamera();
    cam->setCFrame(Matrix4x4(0, 4, 0));

    auto ground = std::make_shared<Model>("res/meshes/ground.obj");
    ground->setSize(Vector4(1, 1, 1));
    ground->setCFrame(Matrix4x4(0, -1.5, 0));
    ground->setColor(Color(0, 1, 0));
    ground->setShader(defaultShader);

    context->addObject(ground);

    auto model = std::make_shared<Model>(meshPath.c_str());
    model->setSize(Vector4(sx, sy, sz));
    model->setCFrame(Matrix4x4(0, 3, -8) * Matrix4x4::fromEuler(0.75, 0.75, 0));
    model->setColor(Color(r, g, b));
    model->setShader(defaultShader);

    context->addObject(model);

    auto model2 = std::make_shared<Model>("res/meshes/blade_thinner.obj");
    model2->setSize(Vector4(0.75, 0.75, 0.75));
    model2->setCFrame(Matrix4x4(8, 3, 0) * Matrix4x4::fromEuler(0.75, 0.75, 0));
    model2->setColor(Color(0.2, 0.2, 0.2));
    model2->setShader(defaultShader);

    context->addObject(model2);

    auto cube = std::make_shared<Cuboid>();
    cube->setSize(Vector4(2, 1, 1));
    cube->setCFrame(Matrix4x4(-8, 3, 0));
    cube->setColor(Color(0.2, 0.2, 0.85));
    cube->setShader(defaultShader);

    context->addObject(cube);

    auto sphere = std::make_shared<Sphere>();
    sphere->setSize(Vector4(2, 2, 2));
    sphere->setCFrame(Matrix4x4(0, 3, 8));
    sphere->setColor(Color(0.2, 0.2, 0.85));
    sphere->setShader(defaultShader);

    context->addObject(sphere);

    auto cube2 = std::make_shared<Cuboid>();
    cube2->setSize(Vector4(0.25, 0.5, 0.25));
    cube2->setColor(Color(0.85, 0.2, 0.2));
    cube2->setShader(defaultShader);

    context->addObject(cube2);

    float x, y, z;
    x = y = z = 0;

    float cX, cY;
    cX = cY = 0;
    window.setMouseDownCallback([&window](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window.setMouseLockEnabled(true);
        }
    });
    window.setMouseUpCallback([&window](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window.setMouseLockEnabled(false);
        }
    });
    window.setMouseMoveCallback([&window, &cX, &cY](int x, int y, int dx, int dy) {
        if (window.isMouseDown(MOUSE_BUTTON::RIGHT)) {
            cX -= dx * MOUSE_SENS;
            cY = std::max(std::min(cY - dy * MOUSE_SENS, 90.0f), -90.0f);
        }
    });
    window.setKeyUpCallback([&window](KEYCODE key) {
        if (key == KEYCODE::F11) {
            window.toggleFullscreen();
        }
    });

    try {
        window.setVSyncEnabled(true);
        std::cout << "Vsync enabled" << std::endl;
    } catch (Exception ex) {
        std::cout << ex << std::endl;
    }

    while (window.isActive()) {
        window.updateViewport();
        window.clear();

        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        cube->setCFrame(Matrix4x4(cube->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
        cube2->setCFrame(cube->getCFrame() * Matrix4x4(0, cube->getSize()[1] / 2 + cube2->getSize()[1] / 2, 0));
        model->setCFrame(Matrix4x4(model->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
        model2->setCFrame(Matrix4x4(model2->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
        sphere->setCFrame(Matrix4x4(sphere->getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        updateCamera(cam, &window, cX, cY);

        context->render();

        window.update();
        window.pollEvents();
    }

    window.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers. This goes
                for all 3D objects, textures, and shaders. */

    SDL_Quit();
}

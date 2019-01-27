#include "mains/mains.hpp"

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

    Window window1(WIDTH, HEIGHT, "Testing1");
    Window window2(WIDTH, HEIGHT, "Testing2", &window1);

    Context3D* context1 = window1.getContext();
    Context3D* context2 = window2.getContext();

    Shader defaultShader = Shader::defaultPerspective();
    context1->addShader(&defaultShader);
    context2->addShader(&defaultShader);

    Camera* cam1 = context1->getCamera();
    cam1->setCFrame(Matrix4x4(0, 4, 0) * Matrix4x4::fromEuler(-0.25, 0, 0));
    Camera* cam2 = context2->getCamera();
    cam2->setCFrame(Matrix4x4(0, 4, 0) * Matrix4x4::fromEuler(-0.25, 0, 0));

    Model ground("res/meshes/ground.obj");
    ground.setSize(Vector4(1, 1, 1));
    ground.setCFrame(Matrix4x4(0, -1.5, 0));
    ground.setColor(Color(0, 1, 0));
    ground.setShader(&defaultShader);

    context1->addObject(&ground);
    context2->addObject(&ground);

    Model model(meshPath.c_str());

    model.setSize(Vector4(sx, sy, sz));
    model.setCFrame(Matrix4x4(0, 3, -8) * Matrix4x4::fromEuler(0.75, 0.75, 0));
    model.setColor(Color(r, g, b));

    context1->addObject(&model);
    context2->addObject(&model);

    Shader testShader("res/test.vert", "res/test.frag");
    context1->addShader(&testShader);
    context2->addShader(&testShader);

    model.setShader(&testShader);

    window1.setVSyncEnabled(true);
    window2.setVSyncEnabled(true);

    float x, y, z;
    x = y = z = 0;

    float cX1, cY1;
    cX1 = cY1 = 0;
    window1.setMouseDownCallback([&window1](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window1.setMouseLockEnabled(true);
        }
    });
    window1.setMouseUpCallback([&window1](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window1.setMouseLockEnabled(false);
        }
    });
    window1.setMouseMoveCallback([&window1, &cX1, &cY1](int x, int y, int dx, int dy) {
        if (window1.isMouseDown(MOUSE_BUTTON::RIGHT)) {
            cX1 -= dx * MOUSE_SENS;
            cY1 = std::max(std::min(cY1 - dy * MOUSE_SENS, 90.0f), -90.0f);
        }
    });
    window1.setKeyUpCallback([&window1](KEYCODE key) {
        if (key == KEYCODE::F11) {
            window1.toggleFullscreen();
        }
    });
    
    float cX2, cY2;
    cX2 = cY2 = 0;
    window2.setMouseDownCallback([&window2](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window2.setMouseLockEnabled(true);
        }
    });
    window2.setMouseUpCallback([&window2](MOUSE_BUTTON button, int x, int y) {
        if (button == MOUSE_BUTTON::RIGHT) { // right mouse button
            window2.setMouseLockEnabled(false);
        }
    });
    window2.setMouseMoveCallback([&window2, &cX2, &cY2](int x, int y, int dx, int dy) {
        if (window2.isMouseDown(MOUSE_BUTTON::RIGHT)) {
            cX2 -= dx * MOUSE_SENS;
            cY2 = std::max(std::min(cY2 - dy * MOUSE_SENS, 90.0f), -90.0f);
        }
    });
    window2.setKeyUpCallback([&window2](KEYCODE key) {
        if (key == KEYCODE::F11) {
            window2.toggleFullscreen();
        }
    });

    while (window1.isActive() || window2.isActive()) {
        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        model.setCFrame(Matrix4x4(model.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        if (window1.isActive()) {
            window1.makeCurrent();
            window1.updateViewport();
            window1.clear();

            updateCamera(cam1, &window1, cX1, cY1);
            context1->render();

            window1.update();
            window1.pollEvents();
        } else {
            window1.close();
        }
        if (window2.isActive()) {
            window2.makeCurrent();
            window2.updateViewport();
            window2.clear();

            updateCamera(cam2, &window2, cX2, cY2);
            context2->render();

            window2.update();
            window2.pollEvents();
        } else {
            window2.close();
        }
    }

    window1.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers */
    window2.close();

    SDL_Quit();
}

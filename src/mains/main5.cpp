#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main5() {
    Window window(WIDTH, HEIGHT, "Render during resize (by callback)");
    std::cout << "Yay window!" << std::endl;

    Context3D* context = window.getContext();

    Shader defaultShader = Shader::defaultPerspective();
    try {
        context->addShader(&defaultShader);
    } catch (Exception ex) {
        std::cout << ex << std::endl;
        exit(-1);
    }
    std::cout << "Yay shader!" << std::endl;

    Cuboid cube;
    cube.setSize(Vector4(1, 1, 1));
    cube.setCFrame(Matrix4x4(0, 0, -5));
    cube.setColor(Color(0.2, 0.2, 0.85));
    cube.setShader(&defaultShader);

    context->addObject(&cube);
    std::cout << "Yay cube!" << std::endl;

    // trying to activate vsync will fail on many x-server based window managers (no vsync support)
    try {
        window.setVSyncEnabled(true);
        std::cout << "Vsync enabled" << std::endl;
    } catch (Exception ex) {
        std::cout << ex << std::endl;
    }

    Camera* cam = context->getCamera();

    float x, y, z;
    x = y = z = 0.0;

    float cX1, cY1;
    cX1 = cY1 = 0;
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
    window.setMouseMoveCallback([&window, &cX1, &cY1](int x, int y, int dx, int dy) {
        if (window.isMouseDown(MOUSE_BUTTON::RIGHT)) {
            cX1 -= dx * MOUSE_SENS;
            cY1 = std::max(std::min(cY1 - dy * MOUSE_SENS, 90.0f), -90.0f);
        }
    });
    window.setKeyUpCallback([&window](KEYCODE key) {
        if (key == KEYCODE::F11) {
            window.toggleFullscreen();
        }
    });

    // here is where the magic happens. in our resize callback we do the render step we would normally do, minus the pollEvents at the end
    // note this, only works while the window is actively being resized. it does not work when the mouse is held down on a window edge, but is
    // not being moved.
    window.setResizeCallback([&](int width, int height) {
        // We must explicitly call applyResize when we set a resize callback or else the window's
        // size won't be kept track of. This is left up to the application programmer when implementing
        // a resize callback so that they can take care of thread safety issues, or so they can discard
        // window resizes if they so choose.
        window.applyResize();

        // do render
        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        window.updateViewport();
        window.clear();

        updateCamera(cam, &window, cX1, cY1);
        context->render();

        window.update();
    });

    while (window.isActive()) {
        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        // we no longer need to do updateViewport in this loop because it is handled in the resize callback
        window.clear();

        updateCamera(cam, &window, cX1, cY1);
        context->render();

        window.update();
        window.pollEvents();
    }

    window.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers */

    SDL_Quit();
}

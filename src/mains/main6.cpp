#include <mutex>
#include <thread>

#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main6() {
    Window window(WIDTH, HEIGHT, "Render during resize (by multithreading)");

    Context3D* context = window.getContext();

    Shader defaultShader = Shader::defaultPerspective();
    context->addShader(&defaultShader);

    Cuboid cube;
    cube.setSize(Vector4(1, 1, 1));
    cube.setCFrame(Matrix4x4(0, 0, -5));
    cube.setColor(Color(0.2, 0.2, 0.85));
    cube.setShader(&defaultShader);

    context->addObject(&cube);

    window.setVSyncEnabled(true);

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
    // The default defined resize callback could work fine for this example.
    // However, the default resize callback will apply the resize automatically for us. This is not likely
    // to be thread safe, as the render thread could be accessing the window size while the resize callback
    // is modifying it. To handle this, we can make it thread safe ourselves, in this case by a mutex and bool.

    bool needResize = false;
    std::mutex mtx;
    window.setResizeCallback([&](int width, int height) {
        std::lock_guard<std::mutex> lck(mtx); // acquire lock
        needResize = true; // tell the render thread that it needs to apply our resize
        // when lock goes out of scope it will unlock the mutex
    });

    // NOTE: While this example for the most part works great for rendering while resizing, it generates a flicker on some hardware and OS combos
    //         such as NVIDIA on Windows 10, and even frame drops for my old laptop with Intel HD graphics and Windows 10.
    //       The flicker is due to the window resize internally clearing the contents of the viewport, so the window stays black until
    //         the next render step occurs in the render thread. This may be good enough for some applications, as it provides
    //         responsive window resizing and uninterrupted rendering (on some hardware). To solve these side effects, check out main7.cpp
    //       Additionally, on my old laptop, this example, even with the above callback does quite poorly. Trying
    //       to render during a resize in this way does poorly, and I suspect it is an issue in the underlying
    //       drives or hardware. However, example main5 and main7 perform well and as expected, if you ignore
    //       the fact that window resizes on that laptop only occur at 30 FPS.


    window.makeCurrent(false); // release rendering control from this thread

    std::thread t([&]() {
        window.makeCurrent(); // take rendering control on this thread
        while (window.isActive()) {
            mtx.lock(); // lock mutex so we can read the needResize value and resize accordingly
            if (needResize) {
                window.applyResize(); // apply the last resize
                window.updateViewport(); // update the render viewport
                needResize = false; // reset variable
            }
            mtx.unlock(); // unlock mutex because we're done

            x = x + 0.01;
            y = y + 0.01;
            z = z + 0.01;

            cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

            // we no longer need to do updateViewport right here because it is handled above
            window.clear();

            updateCamera(cam, &window, cX1, cY1);
            context->render();

            window.update();
            // notice we don't poll events here
        }
        // release rendering control from this thread so that the main thread can clean up resources
        // and close the window
        window.makeCurrent(false);
    });

    while (window.isActive()) {
        window.waitEvents(); // notice we wait for events here instead of polling. this is much more efficient
    }

    t.join();

    window.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers */

    SDL_Quit();
}

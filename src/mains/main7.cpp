#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main7() {
    Window window(WIDTH, HEIGHT, "Render during resize (by multithreading and callback)");

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

	std::mutex mtx; // for the condition variable
    std::condition_variable cv; // variable that will be used to indicate when threads have given up context
    std::atomic<bool> needSync(false); // bool writes and reads should be atomic already, but just in case we will use std::atomic
	std::atomic<bool> goingFullscreen(false);

	window.setKeyUpCallback([&window, &mtx, &cv, &goingFullscreen](KEYCODE key) {
        if (key == KEYCODE::F11) {
			std::unique_lock<std::mutex> lck(mtx);
			goingFullscreen.store(true);
			cv.wait(lck);
			window.toggleFullscreen();
			goingFullscreen.store(false);
			cv.notify_one();
        }
    });

    // This example makes use of the window resize callback and multithreading combined to allow rendering "while" resizing a window.
    // This works by "holding up" the window resize callback until the render thread has had time to process the
    // resize and render. Since we don't let the window resize callback finish until a render has happened, the
    // flicker experienced in example main6 (multiple threads, but no resize callback) is eliminated. Essentially
    // this method syncs up the render and window event threads upon window resize. The only drawback is that
    // the rendering speed then becomes dependent on the speed of the window resizing, so if you are on a slow
    // system where resizing windows is sluggish, the rendering performance will take a hit during resize. This
    // is the price that is paid to have window resizing be synced with the rendering.

    // Here is where the magic happens. In our resize callback we make sure to wait until a resize has occured
    // before we return, so that the threads can sync.
    window.setResizeCallback([&](int width, int height) {
		if (!goingFullscreen.load()) {
			std::unique_lock<std::mutex> lck(mtx); // acquire lock
			needSync.store(true); // indicate to the render thread that it needs to sync up (and apply resize)
			// After the render thread has synced a resize successfully, it waits for the next resize event
			// with a timeout of 7 milliseconds. This is done because if the render thread were to go on to render
			// again before the next resize event was processed, it would slow down resizing; this avoids that.
			// So, we must notify the render thread that another resize is happening, which is what the following
			// line does. Note, if the render thread is not waiting for a resize, this line does nothing.
			cv.notify_one();
			cv.wait(lck); // block this function and thread until the render has completed
		}
    });

    window.makeCurrent(false); // release rendering control from this thread

    std::thread t([&]() {
        window.makeCurrent(); // take rendering control on this thread
        while (window.isActive()) {
            // handle window going fullscreen as well
			if (goingFullscreen.load()) {
				cv.notify_one();
				std::unique_lock<std::mutex> lck(mtx);
				cv.wait(lck);
				window.applyResize();
				window.updateViewport();
			}
			bool shouldNotify = needSync.load(); // store value of needSync at start of render step
			if (shouldNotify) {
				// If the window event thread wants to sync, we know that a resize has occured and therefore
                // we must apply the resize and update the viewport. We must explicitly call applyResize when
				// we set a resize callback or else the window's size won't be kept track of. This is left up
				// to the application programmer when implementing a resize callback so that they can take
				// care of thread safety issues, or so they can discard window resizes if they so choose.
				// In this example, applying the resize here is thread safe because the main window event
				// thread is currently waiting for this render thread to render, so it cannot be modifying or
				// reading the window at this point.
				window.applyResize();
				window.updateViewport();
			}

            x = x + 0.01;
            y = y + 0.01;
            z = z + 0.01;

            cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

            // we no longer need to do updateViewport right here because it is handled above
            window.clear();

            updateCamera(cam, &window, cX, cY);
            context->render();

			window.update();
			if (shouldNotify) {
				needSync.store(false); // reset sync variable
				cv.notify_one(); // tell window event thread that this thread has rendered with the updated window size
				std::unique_lock<std::mutex> lck(mtx); // acquire lock
				cv.wait_for(lck, std::chrono::milliseconds(7)); // try to wait for next resize, timeout if one doesn't come in 7 milliseconds
                // 7 is the magic number ;)
            }
            // notice we don't poll events here, the main window event thread handles that
        }
        // release rendering control from this thread so that the main thread can clean up resources
        // and close the window
        window.makeCurrent(false);
    });

    while (window.isActive()) {
        window.waitEvents(); // notice we wait for events here instead of polling. this is much more efficient (avoids a busy loop)
    }

    t.join();

    window.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers */

    SDL_Quit();
}

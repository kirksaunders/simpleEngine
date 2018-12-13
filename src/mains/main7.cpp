#include <thread>
#include <atomic>

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

	std::atomic<bool> transferContext(false); // bool writes and reads should be atomic already, but just in case we will use std::atomic

	// This example makes use of the window resize callback and multithreading combined to allow rendering "while" resizing a window.
	// The catch with this is that an OpenGL context is not able to be made current in more than one thread at a time, so the render thread must
	// give up the context to the main thread so that it may render upon window resize. One side effect of this method is that resizing the window
	// can start to feel a little bit laggy or sluggish when the rendering function takes too long to give up the thread. One solution could potentially be
	// to turn off vsync for the whole application, or just while the window is being resized.

	// Here is where the magic happens. In our resize callback we do the render step we would normally do, minus the pollEvents at the end.
	// But we must additionally gain ownership of the context before we can render it.
	window.setResizeCallback([&](int width, int height) {
		transferContext.store(true); // indicate to the render thread that we need the context
        while (transferContext.load()) {
            std::this_thread::yield(); // yield until the render thread has released the context
        }
        window.makeCurrent(); // claim the context on this thread

		window.applyResize(width, height); // we must explicitly call this when we set a resize callback or else the window's size won't be kept track of
		// the reason this is the case is because setResizeCallback leaves it up to the programmer so that they can do any thread safety stuff, like we are doing now

        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

        window.updateViewport();
        window.clear();

        updateCamera(cam, &window, cX1, cY1);
        context->render();

        window.update();
		window.makeCurrent(false);
        transferContext.store(true); // indicate that the render thread can have the context back
		while (transferContext.load()) {
            std::this_thread::yield(); // yield until the render thread has reset transferContext to false
        }
    });

	window.makeCurrent(false); // release rendering control from this thread

	std::thread t([&]() {
		window.makeCurrent(); // take rendering control on this thread
		while (window.isActive()) {
            if (transferContext.load()) { // main thread wants the context
                window.makeCurrent(false); // releast context
                transferContext.store(false); // indicate this thread has released the context
                while (!transferContext.load()) {
                    std::this_thread::yield(); // yield until the main thread has released the context
                }
                transferContext.store(false); // reset variable for next time
				window.makeCurrent(); // take context back
				continue; // go ahead and skip back to beginning of loop (and also check if window is still active)
            }

			x = x + 0.01;
			y = y + 0.01;
			z = z + 0.01;

			cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));

			// we no longer need to do updateViewport in this loop because it is handled in the resize callback
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

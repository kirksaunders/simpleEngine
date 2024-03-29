#include "mains/mains.hpp"

#include "render_base/texturebuffer.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void raytracer() {
    using namespace std;

    Window window(WIDTH, HEIGHT, "Testing");
    window.setVSyncEnabled(true);
    Context3D* context = window.getContext();
    Camera* camera = context->getCamera();
    Matrix4x4 camCFr = camera->getCFrame();
    //Matrix4x4 camInverse = camCFr.inverse();
    Matrix4x4 proj = camera->getPerspective(static_cast<float>(WIDTH) / HEIGHT);
    Matrix4x4 projInverse = proj.inverse();

    Vector4 forward(0.2, 0.2, -1); //camCFr.lookvector();
    forward = forward.unit();
    cout << "Forward: " << forward[0] << ", " << forward[1] << ", " << forward[2] << endl;

    TextureBuffer buffer(WIDTH, HEIGHT, 4);

    Color red(1, 0, 0);
    Color black(0, 0, 0);

    for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
            Vector4 screenVec;
            screenVec[0] = 2.0 * x / (WIDTH - 1) - 1.0;
            screenVec[1] = 2.0 * (1.0 - static_cast<float>(y) / (HEIGHT - 1)) - 1.0;
            screenVec[2] = 1.0;
            screenVec[3] = 1.0;

            Vector4 dir = (projInverse * screenVec);
            //dir = dir / dir[3];
            dir = dir.unit();

            if (dir.dot(forward) > 0.975) {
                buffer.setPixel(x, y, red * (dir.dot(forward) - 0.975) * 40);
            } else {
                buffer.setPixel(x, y, black);
            }
        }
    }

    Shader defaultShader = Shader::defaultImage();
    context->addShader(&defaultShader);

    Texture tex(buffer);
    context->addTexture(&tex);
    tex.setShader(&defaultShader);

    while (window.isActive()) {
        window.updateViewport();
        window.clear();

        context->renderTexture(tex);

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

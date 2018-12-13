#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

void main2() {
    std::string imagePath;
    std::cout << "Enter image file path: ";
    std::getline(std::cin, imagePath);

    Window window1(WIDTH, HEIGHT, "Testing1");
    Window window2(WIDTH, HEIGHT, "Testing2", &window1);

    Context3D* context1 = window1.getContext();
    Context3D* context2 = window2.getContext();

    Shader defaultShader = Shader::defaultImage();
    context1->addShader(&defaultShader);
    context2->addShader(&defaultShader);

    Texture tex(imagePath.c_str());
    tex.setShader(&defaultShader);
    context1->addTexture(&tex);
    context2->addTexture(&tex);

    window1.setVSyncEnabled(true);
    window2.setVSyncEnabled(true);

    while (window1.isActive() || window2.isActive()) {
        if (window1.isActive()) {
            window1.makeCurrent();
            window1.updateViewport();
            window1.clear();

            context1->renderTexture(tex);

            window1.update();
            window1.pollEvents();
        } else {
            window1.close();
        }
        if (window2.isActive()) {
            window2.makeCurrent();
            window2.updateViewport();
            window2.clear();

            context2->renderTexture(tex);

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

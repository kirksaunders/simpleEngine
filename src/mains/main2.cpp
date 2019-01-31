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

    Shader defaultImageShader = Shader::defaultImage();
    context1->addShader(&defaultImageShader);
    context2->addShader(&defaultImageShader);

    Shader defaultObjectShader = Shader::defaultPerspective();
    context1->addShader(&defaultObjectShader);
    context2->addShader(&defaultObjectShader);

    Texture tex(imagePath.c_str());
    tex.setShader(&defaultImageShader);
    context1->addTexture(&tex);
    context2->addTexture(&tex);

    Cuboid cube;
    cube.setShader(&defaultObjectShader);
    cube.setSize(Vector4(1, 1, 1));
    cube.setCFrame(Matrix4x4(0, 0, -5));
    cube.setColor(Color(0, 0, 0.75));
    context1->addObject(&cube);
    context2->addObject(&cube);

    window1.setVSyncEnabled(true);
    window2.setVSyncEnabled(true);

    float x, y, z;
    x = y = z = 0;

    while (window1.isActive() || window2.isActive()) {
        x = x + 0.01;
        y = y + 0.01;
        z = z + 0.01;

        cube.setCFrame(Matrix4x4(cube.getCFrame().position()) * Matrix4x4::fromEuler(x, y, z));
        
        if (window1.isActive()) {
            window1.makeCurrent();
            window1.updateViewport();
            window1.clear();

            context1->renderTexture(tex);
            context1->render(); // the cube will render on top of the texture

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

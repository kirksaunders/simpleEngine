#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

int main(int arc, char** arcv) {
    int option;
    std::cout << "1.) Model loading" << std::endl
              << "2.) Texture loading (multiple windows)" << std::endl
              << "3.) Cuboids (multiple windows)" << std::endl
              << "4.) Mesh loading (multiple windows)" << std::endl
              << "5.) Render while resizing (using callback)" << std::endl
              << "6.) Render while resizing (using two threads)" << std::endl
              << "7.) Render while resizing (using two threads and callback)" << std::endl
              << "8.) Raytracer" << std::endl
              << "9.) Pathfind" << std::endl
              << "Enter your choice for situation to run: ";
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (option) {
     case 1:
        main1();
        break;
     case 2:
        //main2();
        break;
     case 3:
        //main3();
        break;
     case 4:
        //main4();
        break;
     case 5:
         //main5();
        break;
     case 6:
         //main6();
        break;
     case 7:
         //main7();
        break;
     case 8:
        //raytracer();
        break;
     case 9:
        //pathfind();
        break;
     default:
        std::cout << "Invalid choice" << std::endl;
        break;
    }

    return 0;
}

void updateCamera(Camera* cam, Window* window, float cX, float cY) {
    bool w, a, s, d, q, e;
    w = window->isKeyPressed(KEYCODE::W);
    a = window->isKeyPressed(KEYCODE::A);
    s = window->isKeyPressed(KEYCODE::S);
    d = window->isKeyPressed(KEYCODE::D);
    q = window->isKeyPressed(KEYCODE::Q);
    e = window->isKeyPressed(KEYCODE::E);

    Matrix4x4 offset(0, 0, 0);
    if (w) {
        offset = offset * Matrix4x4(0, 0, -DELTA);
    }
    if (s) {
        offset = offset * Matrix4x4(0, 0, DELTA);
    }
    if (a) {
        offset = offset * Matrix4x4(-DELTA, 0, 0);
    }
    if (d) {
        offset = offset * Matrix4x4(DELTA, 0, 0);
    }
    if (q) {
        offset = offset * Matrix4x4(0, -DELTA, 0);
    }
    if (e) {
        offset = offset * Matrix4x4(0, DELTA, 0);
    }
    Matrix4x4 newCFr = cam->getCFrame() * offset;
    cam->setCFrame(Matrix4x4(newCFr.position())
                    * Matrix4x4::fromEuler(0, RAD*cX, 0)
                    * Matrix4x4::fromEuler(RAD*cY, 0, 0));
}

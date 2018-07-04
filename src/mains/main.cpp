#include "mains/mains.hpp"

using std::size_t;
using namespace Render3D;
using namespace Math3D;

static const float DELTA = 0.1;
static const float MOUSE_SENS = 0.15;
static const float RAD = PI / 180.0;

int main() {
    int option;
    std::cout << "1.) Model loading" << std::endl
              << "2.) Texture loading (multiple windows)" << std::endl
              << "3.) Cuboids (multiple windows)" << std::endl
              << "4.) Mesh loading (multiple windows)" << std::endl
              << "5.) Raytracer" << std::endl
              << "6.) Pathfind" << std::endl
              << "Enter your choice for situation to run: ";
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (option) {
     case 1:
        main1();
        break;
     case 2:
        main2();
        break;
     case 3:
        main3();
        break;
     case 4:
        main4();
        break;
     case 5:
        raytracer();
        break;
     case 6:
        pathfind();
        break;
     default:
        std::cout << "Invalid choice" << std::endl;
        break;
    }

    return 0;
}

void updateCamera(Camera* cam, Window* window, float cX, float cY) {
    bool w, a, s, d, q, e;
    w = window->isKeyPressed('W');
    a = window->isKeyPressed('A');
    s = window->isKeyPressed('S');
    d = window->isKeyPressed('D');
    q = window->isKeyPressed('Q');
    e = window->isKeyPressed('E');

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
                    * Matrix4x4::fromEuler(0, RAD*MOUSE_SENS*cX, 0)
                    * Matrix4x4::fromEuler(RAD*MOUSE_SENS*cY, 0, 0));
}

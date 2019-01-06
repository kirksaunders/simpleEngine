#include <cstdlib>
#include <fstream>
#include <vector>

#include "mains/mains.hpp"

#include "astar/grid.hpp"
#include "astar/node.hpp"

#include "render_base/exception.hpp"

using std::size_t;
using namespace Math3D;
using namespace Render3D;

static const float GRID_SIZE = 2.0;

Node* astar(Grid& grid, Node* start, Node* end) {
    std::vector<Node*> open;
    std::vector<Node*> closed;
    Node* current = start;
    while (current != nullptr) {
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    if (x != 0 || y != 0 || z != 0) {
                        Node* node = grid.getNode(current->getPosition() + Vector4(x, y, z));
                        if (node != nullptr) {
                            bool found = false;
                            for (int i = 0; i < closed.size(); ++i) {
                                if (closed[i] == node) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) { 
                                for (int i = 0; i < open.size(); ++i) {
                                    if (open[i] == node) {
                                        found = true;
                                        break;
                                    }
                                }
                                if (found) {
                                    node->updateParent(current);
                                } else {
                                    node->updateParent(current);
                                    node->updateH(end);
                                    open.push_back(node);
                                }
                                if (node == end) {
                                    return end;
                                }
                            }
                        }
                    }
                }
            }
        }
        Node* best = nullptr;
        unsigned int bestIndex;
        for (int i = 0; i < open.size(); ++i) {
            Node* cur = open[i];
            if (best == nullptr || cur->getCost() < best->getCost()) {
                best = cur;
                bestIndex = i;
            }
        }
        closed.push_back(current);
        current = best;
        if (best != nullptr) {
            open.erase(open.begin() + bestIndex);
        }
    }
    return nullptr;
}

void pathfind() {
    /*Grid grid;
    std::ifstream ifs("res/astargrid.txt");
    if (ifs.fail()) {
        std::cout << "Failed to open input file" << std::endl;
        exit(-1);
    }
    grid.load(ifs);
    ifs.close();*/

    Grid grid(20, 10, 20);
    for (int x = -20; x <= 20; x++) {
        for (int y = -10; y <= 10; y++) {
            for (int z = -20; z <= 20; z++) {
                if (y == 0 || (x <= 5 && x >= -5 && y > 0 && y <= 10 && z == 0)) {
                    grid.setNode(x, y, z, nullptr);
                } else {
                    grid.setNode(x, y, z, new Node(Vector4(x, y, z)));
                }
            }
        }
    }

    std::ofstream ofs("res/astargrid.txt");
    if (ofs.fail()) {
        std::cout << "Failed to open output file" << std::endl;
        exit(-1);
    }
    grid.save(ofs);
    ofs.close();

    Window window(WIDTH, HEIGHT, "A* Pathfinding");

    Context3D* context = window.getContext();

    Shader defaultShader = Shader::defaultPerspective();
    try {
        context->addShader(&defaultShader);
    } catch (Exception& ex) { // catch shader compilation issues
        std::cout << ex << std::endl;
        exit(-1);
    }

    Camera* cam = context->getCamera();

    std::vector<Cuboid*> walls;
    Node* node;
    for (int x = -20; x <= 20; x++) {
        for (int y = -10; y <= 10; y++) {
            for (int z = -20; z <= 20; z++) {
                node = grid.getNode(x, y, z);
                if (node == nullptr) {
					Cuboid* cube = new Cuboid();
					cube->setSize(Vector4(GRID_SIZE, GRID_SIZE, GRID_SIZE));
					cube->setCFrame(Matrix4x4(x * GRID_SIZE, y * GRID_SIZE, z * GRID_SIZE));
					cube->setColor(Color(0.75, 0.75, 0.75));
					cube->setShader(&defaultShader);
					context->addObject(cube);
					walls.push_back(cube);
                }
            }
        }
    }

    std::vector<Sphere*> paths;
    Node* path = astar(grid, grid.getNode(0, 2, 5), grid.getNode(0, 2, -5));
    while (path != nullptr) {
        std::cout << path->getPosition() << std::endl;
        Sphere* sphere = new Sphere();
        sphere->setSize(Vector4(GRID_SIZE, GRID_SIZE, GRID_SIZE));
        sphere->setCFrame(Matrix4x4(path->getPosition() * GRID_SIZE));
        sphere->setColor(Color(0, 0.8, 0));
        sphere->setShader(&defaultShader);
        context->addObject(sphere);
        path = path->getParent();
    }

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
    window.setKeyUpCallback([&window](KEYCODE key) {
        if (key == KEYCODE::F11) {
            window.toggleFullscreen();
        }
    });

    window.setVSyncEnabled(false);

    while (window.isActive()) {
        window.updateViewport();
        window.clear();

        updateCamera(cam, &window, cX, cY);

        context->render();

        window.update();
        window.pollEvents();
    }

    window.close(); // window must be closed to clean up resources it uses
    /* WARNING: closing a window requires the objects it currently uses to still exist,
                so do NOT delete an object or let it go out of scope until it has been removed
                from all contexts or all windows that use it have been closed. in the future I will
                likely implement the Context3D to use shared_ptrs instead of raw c pointers */

    for (int i = 0; i < walls.size(); ++i) {
        delete walls[i];
    }
    for (int i = 0; i < paths.size(); ++i) {
        delete paths[i];
    }
}

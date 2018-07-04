#include <fstream>
#include <cstdlib>
#include <vector>

#include "mains/mains.hpp"
#include "astar/node.hpp"
#include "astar/grid.hpp"

using std::size_t;
using namespace Math3D;
using namespace Render3D;

static const float GRID_SIZE = 2.0;

Node* astar(Grid& grid, Node* start, Node* end) {
    std::vector<Node*> open;
    std::vector<Node*> closed;
    Node* current = start;
    while (current != NULL) {
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    if (x != 0 || y !=0 || z != 0) {
                        Node* node = grid.getNode(current->getPosition() + Vector4(x, y, z));
                        if (node != NULL) {
                            bool found = false;
                            for (int i = 0; i < closed.size(); i++) {
                                if (closed[i] == node) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) { 
                                for (int i = 0; i < open.size(); i++) {
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
        Node* best = NULL;
        unsigned int bestIndex;
        for (int i = 0; i < open.size(); i++) {
            Node* cur = open[i];
            if (best == NULL || cur->getCost() < best->getCost()) {
                best = cur;
                bestIndex = i;
            }
        }
        closed.push_back(current);
        current = best;
        if (best != NULL) {
            open.erase(open.begin() + bestIndex);
        }
    }
    return NULL;
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
                    grid.setNode(x, y, z, NULL);
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

    Window* window = new Window(WIDTH, HEIGHT, "A* Pathfinding");

	Context3D context = Context3D(window);

    Camera* cam = context.getCamera();

    std::vector<Cuboid*> walls;
    Node* node;
    for (int x = -20; x <= 20; x++) {
        for (int y = -10; y <= 10; y++) {
            for (int z = -20; z <= 20; z++) {
                node = grid.getNode(x, y, z);
                if (node == NULL) {
                    Cuboid* cube = new Cuboid();
                    cube->setSize(Vector4(GRID_SIZE, GRID_SIZE, GRID_SIZE));
                    cube->setCFrame(Matrix4x4(x * GRID_SIZE, y * GRID_SIZE, z * GRID_SIZE));
                    cube->setColor(Color(0.75, 0.75, 0.75));
                    context.addObject(cube);
                    walls.push_back(cube);
                }
            }
        }
    }

    std::vector<Sphere*> paths;
    Node* path = astar(grid, grid.getNode(0, 2, 5), grid.getNode(0, 2, -5));
    while (path != NULL) {
        std::cout << path->getPosition() << std::endl;
        Sphere* sphere = new Sphere();
        sphere->setSize(Vector4(GRID_SIZE, GRID_SIZE, GRID_SIZE));
        sphere->setCFrame(Matrix4x4(path->getPosition() * GRID_SIZE));
        sphere->setColor(Color(0, 0.8, 0));
        context.addObject(sphere);
        path = path->getParent();
    }

    float cX, cY;
    cX = 0;
    cY = -100;
    bool rightMouseDown = false;
    double mX, mY;
    window->setMouseDownCallback([&window, &rightMouseDown, &mX, &mY](int button) {
        if (button == 1) { // right mouse button
            window->setMouseLockEnabled(true);
            rightMouseDown = true;
            window->getMousePosition(mX, mY);
        }
    });
    window->setMouseUpCallback([&window, &rightMouseDown](int button) {
        if (button == 1) { // right mouse button
            window->setMouseLockEnabled(false);
            rightMouseDown = false;
        }
    });
    window->setMouseMoveCallback([&rightMouseDown, &mX, &mY, &cX, &cY](double x, double y) {
        if (rightMouseDown) {
            cX -= (x - mX);
            cY -= (y - mY);
            mX = x;
            mY = y;
        }
    });

	window->setVSyncEnabled(false);

	while (window->isActive()) {
		window->clear();

        updateCamera(cam, window, cX, cY);

		context.render();

		window->update();
		window->pollEvents();
	}

	delete window;
    for (int i = 0; i < walls.size(); i++) {
        delete walls[i];
    }
    for (int i = 0; i < paths.size(); i++) {
        delete paths[i];
    }
}

#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>

#include "astar/node.hpp"

#include "math/vector4.hpp"

class Grid {
 public:
    Grid();
    Grid(int x, int y, int z);
    ~Grid();
    void setNode(int x, int y, int z, Node* node);
    Node* getNode(int x, int y, int z) const;
    Node* getNode(const Math3D::Vector4 pos) const;
    void load(std::istream& ins);
    void save(std::ostream& outs);
 private:
    Node** grid;
    int width, height, depth;
    void destroy();
};

#endif
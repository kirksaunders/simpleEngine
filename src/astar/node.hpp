#ifndef NODE_HPP
#define NODE_HPP

#include "math/vector4.hpp"

class Node {
 public:
    Node();
    Node(const Math3D::Vector4 pos);
    Node(const Math3D::Vector4 pos, Node* par, const Node* end);

    void setPosition(const Math3D::Vector4 pos);
    Math3D::Vector4 getPosition() const;
    double getDistanceTo(const Math3D::Vector4 pos) const;
    double getPathDistance(const Math3D::Vector4 pos) const;
    void updateH(const Node* end);
    void updateParent(Node* par);
    Node* getParent() const;
    double getCost() const;

    bool operator<(const Node& other) const;
 private:
    Node* parent;
    Math3D::Vector4 position;
    double h;
    double g;
};

#endif
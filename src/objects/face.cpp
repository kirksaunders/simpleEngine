#include "objects/face.hpp"

using namespace Render3D;
using namespace Math3D;

Face::Face(std::vector<Vector4>& verts, const Color& clr) {
    color = clr;
    for (int i = 2; i < verts.size(); ++i) {
        triangles.push_back(Triangle(verts[0], verts[i-1], verts[i], clr));
    }
}

void Face::update(std::vector<Vector4>& verts, const Color& clr) {
    color = clr;

    triangles.clear();
    for (int i = 2; i < verts.size(); ++i) {
        triangles.push_back(Triangle(verts[0], verts[i-1], verts[i], clr));
    }
}

std::vector<Triangle> Face::getTriangles() const {
    return triangles;
}

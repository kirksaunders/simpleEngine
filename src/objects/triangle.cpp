#include "objects/triangle.hpp"

using namespace Render3D;
using namespace Math3D;

Triangle::Triangle(const Vector4& a, const Vector4& b, const Vector4& c, const Color& d) {
    v1 = a;
    v2 = b;
    v3 = c;
    color = d;
    center = (a + b + c) / 3;
    normal = (b-a).cross(c-a).unit();
}

Vector4 Triangle::getVertex1() const {
    return v1;
}

Vector4 Triangle::getVertex2() const {
    return v2;
}

Vector4 Triangle::getVertex3() const {
    return v3;
}

Vector4 Triangle::getCenter() const {
    return center;
}

Vector4 Triangle::getNormal() const {
    return normal;
}

Color Triangle::getColor() const {
    return color;
}

void Triangle::setDistanceCache(float dis) {
    distanceCache = dis;
}

float Triangle::getDistanceCache() const {
    return distanceCache;
}

bool Triangle::operator<(const Triangle& tri) const {
    return distanceCache > tri.getDistanceCache();
}

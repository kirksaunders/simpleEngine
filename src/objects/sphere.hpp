#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <GL/glew.h>

#include "objects/model.hpp"
#include "objects/primitive3d.hpp"

namespace Render3D {
    class Sphere : public Model {
     public:
        Sphere() : Model("res/meshes/sphere.obj") {}
    };
};

#endif
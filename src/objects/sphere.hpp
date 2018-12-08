#ifndef SPHERE_HPP
#define SPHERE_HPP

#define GLEW_STATIC

#include <GLEW/glew.h>

#include "objects/primitive3d.hpp"
#include "objects/model.hpp"

namespace Render3D {
	class Sphere : public Model {
	 public:
		Sphere() : Model("res/meshes/sphere.obj") {}
	};
};

#endif
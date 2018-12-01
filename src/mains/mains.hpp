#ifndef MAINS_HPP
#define MAINS_HPP

#include <iostream>
#include <limits>
#include <cstdlib>

#include "math/math.hpp"

#include "render_base/window.hpp"
#include "render_base/context3d.hpp"
#include "objects/model.hpp"
#include "objects/cuboid.hpp"
#include "objects/sphere.hpp"

const static size_t WIDTH = 600;
const static size_t HEIGHT = 600;

static const float DELTA = 0.1;
static const float MOUSE_SENS = 0.15;
static const float RAD = PI / 180.0;

void main1();
void main2();
void main3();
void main4();
void raytracer();
void updateCamera(Render3D::Camera* cam, Render3D::Window* window, float cX, float cY);
void pathfind();

#endif
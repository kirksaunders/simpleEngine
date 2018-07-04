#ifndef FACE_HPP
#define FACE_HPP

#include <vector>

#include "objects/triangle.hpp"
#include "math/vector4.hpp"

#include "render_base/color.hpp"

namespace Render3D {
	class Face {
	 public:
		Face(std::vector<Math3D::Vector4>& verts, const Color& clr);

		void update(std::vector<Math3D::Vector4>& verts, const Color& clr);

		std::vector<Triangle> getTriangles() const;

	 private:
		std::vector<Triangle> triangles;
		Color color;
	};
}

#endif
#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "math/vector4.hpp"

#include "render_base/color.hpp"

namespace Render3D {
	class Triangle {
	 public:
		Triangle(const Math3D::Vector4& a, const Math3D::Vector4& b, const Math3D::Vector4& c, const Color& d);

		Math3D::Vector4 getVertex1() const;

		Math3D::Vector4 getVertex2() const;

		Math3D::Vector4 getVertex3() const;

		Math3D::Vector4 getCenter() const;

		Math3D::Vector4 getNormal() const;

		Color getColor() const;

		void setDistanceCache(float dis);

		float getDistanceCache() const;

		bool operator<(const Triangle& tri) const;

	 private:
		Math3D::Vector4 v1;
		Math3D::Vector4 v2;
		Math3D::Vector4 v3;

		Math3D::Vector4 center;

		Math3D::Vector4 normal;

		Color color;

		float distanceCache;
	};
}

#endif
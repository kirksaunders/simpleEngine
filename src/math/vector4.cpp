#include <cmath>

#include "math/constants.hpp"
#include "math/vector4.hpp"

using namespace Math3D;

Vector4::Vector4() {
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	values[3] = 1;
}

Vector4::Vector4(float x, float y, float z) {
	values[0] = x;
	values[1] = y;
	values[2] = z;
	values[3] = 1;
}

float Vector4::dot(const Vector4& b) const {
	return (values[0] * b[0] + values[1] * b[1] + values[2] * b[2]);
}

Vector4 Vector4::cross(const Vector4& b) const {
	Vector4 out;

	out.values[0] = values[1] * b[2] - values[2] * b[1];
	out.values[1] = values[2] * b[0] - values[0] * b[2];
	out.values[2] = values[0] * b[1] - values[1] * b[0];
	out.values[3] = 1;

	return out;
}

float Vector4::magnitude() const {
	return sqrt(values[0] * values[0] + values[1] * values[1] + values[2] * values[2]);
}

Vector4 Vector4::unit() const {
	return operator/(magnitude());
}

Vector4 Vector4::operator*(float b) const {
	Vector4 out;

	out.values[0] = values[0] * b;
	out.values[1] = values[1] * b;
	out.values[2] = values[2] * b;
	out.values[3] = values[3] * b;

	return out;
}

Vector4 Vector4::operator*(const Vector4& b) const {
	Vector4 out;

	out.values[0] = values[0] * b[0];
	out.values[1] = values[1] * b[1];
	out.values[2] = values[2] * b[2];
	out.values[3] = values[3] * b[3];

	return out;
}

Vector4 Vector4::operator/(float b) const {
	Vector4 out;

	out.values[0] = values[0] / b;
	out.values[1] = values[1] / b;
	out.values[2] = values[2] / b;
	out.values[3] = values[3] / b;

	return out;
}

Vector4 Vector4::operator/(const Vector4& b) const {
	Vector4 out;

	out.values[0] = values[0] / b[0];
	out.values[1] = values[1] / b[1];
	out.values[2] = values[2] / b[2];
	out.values[3] = values[3] / b[3];

	return out;
}

Vector4 Vector4::operator+(const Vector4& b) const {
	Vector4 out;

	out.values[0] = values[0] + b[0];
	out.values[1] = values[1] + b[1];
	out.values[2] = values[2] + b[2];
	out.values[3] = values[3] + b[3];

	return out;
}

Vector4 Vector4::operator-(const Vector4& b) const {
	Vector4 out;

	out.values[0] = values[0] - b[0];
	out.values[1] = values[1] - b[1];
	out.values[2] = values[2] - b[2];
	out.values[3] = values[3] - b[3];

	return out;
}

bool Vector4::operator==(const Vector4& b) const {
	for (unsigned int i = 0; i < 4; i++) {
		if (fabs(values[i] - b.values[i]) > EPSILON) {
			return false;
		}
	}
	return true;
}

bool Vector4::operator!=(const Vector4& b) const {
	for (unsigned int i = 0; i < 4; i++) {
		if (fabs(values[i] - b.values[i]) > EPSILON) {
			return true;
		}
	}
	return false;
}

float& Vector4::operator[](int i) {
	return values[i];
}

const float Vector4::operator[](int i) const {
	return values[i];
}

float* Vector4::getValues() {
	return values;
}

std::ostream& Math3D::operator <<(std::ostream &outs, const Vector4 &vec) {
	outs << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3];
}



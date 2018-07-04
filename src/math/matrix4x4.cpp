#include <cmath>

#include "math/matrix4x4.hpp"

using namespace Math3D;

Matrix4x4::Matrix4x4() {
	values[0][0] = (float)1;
	values[0][1] = 0;
	values[0][2] = 0;
	values[0][3] = 0;
	values[1][0] = 0;
	values[1][1] = (float)1;
	values[1][2] = 0;
	values[1][3] = 0;
	values[2][0] = 0;
	values[2][1] = 0;
	values[2][2] = (float)1;
	values[2][3] = 0;
	values[3][0] = 0;
	values[3][1] = 0;
	values[3][2] = 0;
	values[3][3] = (float)1;
}

Matrix4x4::Matrix4x4(const Vector4& vector) {
	values[0][0] = (float)1;
	values[0][1] = 0;
	values[0][2] = 0;
	values[0][3] = vector[0];
	values[1][0] = 0;
	values[1][1] = (float)1;
	values[1][2] = 0;
	values[1][3] = vector[1];
	values[2][0] = 0;
	values[2][1] = 0;
	values[2][2] = (float)1;
	values[2][3] = vector[2];
	values[3][0] = 0;
	values[3][1] = 0;
	values[3][2] = 0;
	values[3][3] = (float)1;
}

Matrix4x4::Matrix4x4(float x, float y, float z) {
	values[0][0] = (float)1;
	values[0][1] = 0;
	values[0][2] = 0;
	values[0][3] = x;
	values[1][0] = 0;
	values[1][1] = (float)1;
	values[1][2] = 0;
	values[1][3] = y;
	values[2][0] = 0;
	values[2][1] = 0;
	values[2][2] = (float)1;
	values[2][3] = z;
	values[3][0] = 0;
	values[3][1] = 0;
	values[3][2] = 0;
	values[3][3] = (float)1;
}

Matrix4x4::Matrix4x4(float x, float y, float z, float r00, float r01, float r02, float r10, float r11, float r12, float r20, float r21, float r22) {
	values[0][3] = x;
	values[1][3] = y;
	values[2][3] = z;

	values[0][0] = r00;
	values[0][1] = r01;
	values[0][2] = r02;
	values[1][0] = r10;
	values[1][1] = r11;
	values[1][2] = r12;
	values[2][0] = r20;
	values[2][1] = r21;
	values[2][2] = r22;

	values[3][0] = 0;
	values[3][1] = 0;
	values[3][2] = 0;
	values[3][3] = 1;
}

Matrix4x4 Matrix4x4::fromEuler(float x, float y, float z) {
	Matrix4x4 out;

	out.values[0][0] = cos(y) * cos(z);
	out.values[0][1] = -sin(z) * cos(y);
	out.values[0][2] = sin(y);
	out.values[0][3] = 0;
	out.values[1][0] = cos(z) * sin(y) * sin(x) + sin(z) * cos(x);
	out.values[1][1] = cos(z) * cos(x) - sin(z) * sin(y) * sin(x);
	out.values[1][2] = -cos(y) * sin(x);
	out.values[1][3] = 0;
	out.values[2][0] = sin(z) * sin(x) - cos(z) * sin(y) * cos(x);
	out.values[2][1] = sin(z) * sin(y) * cos(x) + cos(z) * sin(x);
	out.values[2][2] = cos(y) * cos(x);
	out.values[2][3] = 0;
	out.values[3][0] = 0;
	out.values[3][1] = 0;
	out.values[3][2] = 0;
	out.values[3][3] = 1;

	return out;
}

Matrix4x4 Matrix4x4::projection(float fov, float aspectRatio, float near, float far) {
	float tanHalfFOV = tan(fov * 3.14159265359 / 180 / 2);
	float range = far - near;

	Matrix4x4 out;

	out.values[0][0] = 1 / (tanHalfFOV * aspectRatio);
	out.values[0][1] = 0;
	out.values[0][2] = 0;
	out.values[0][3] = 0;
	out.values[1][0] = 0;
	out.values[1][1] = 1 / tanHalfFOV;
	out.values[1][2] = 0;
	out.values[1][3] = 0;
	out.values[2][0] = 0;
	out.values[2][1] = 0;
	out.values[2][2] = -(far + near) / range;
	out.values[2][3] = -2 * far * near / range;
	out.values[3][0] = 0;
	out.values[3][1] = 0;
	out.values[3][2] = -1;
	out.values[3][3] = 0;

	return out;
}

Matrix4x4 Matrix4x4::inverse() const {
	float det = -values[0][2] * values[1][1] * values[2][0] + values[0][1] * values[1][2] * values[2][0]
				+ values[0][2] * values[1][0] * values[2][1] - values[0][0] * values[1][2] * values[2][1]
				- values[0][1] * values[1][0] * values[2][2] + values[0][0] * values[1][1] * values[2][2];
	float k = 1/det;
			
	Matrix4x4 out;

	out.values[0][0] = (values[1][1] * values[2][2] - values[2][1] * values[1][2]) * k;
	out.values[0][1] = (values[2][1] * values[0][2] - values[0][1] * values[2][2]) * k;
	out.values[0][2] = (values[0][1] * values[1][2] - values[1][1] * values[0][2]) * k;
	out.values[1][0] = (values[1][2] * values[2][0] - values[2][2] * values[1][0]) * k;
	out.values[1][1] = (values[2][2] * values[0][0] - values[0][2] * values[2][0]) * k;
	out.values[1][2] = (values[0][2] * values[1][0] - values[1][2] * values[0][0]) * k;
	out.values[2][0] = (values[1][0] * values[2][1] - values[2][0] * values[1][1]) * k;
	out.values[2][1] = (values[2][0] * values[0][1] - values[0][0] * values[2][1]) * k;
	out.values[2][2] = (values[0][0] * values[1][1] - values[1][0] * values[0][1]) * k;

	out.values[0][3] = -(out.values[0][0] * values[0][3] + out.values[0][1] * values[1][3] + out.values[0][2] * values[2][3]);
	out.values[1][3] = -(out.values[1][0] * values[0][3] + out.values[1][1] * values[1][3] + out.values[1][2] * values[2][3]);
	out.values[2][3] = -(out.values[2][0] * values[0][3] + out.values[2][1] * values[1][3] + out.values[2][2] * values[2][3]);

	out.values[3][0] = values[3][0];
	out.values[3][1] = values[3][1];
	out.values[3][2] = values[3][2];
	out.values[3][3] = values[3][3];

	return out;
}

Matrix4x4 Matrix4x4::transpose() const {
	Matrix4x4 out;

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			out.values[column][row] = values[row][column];
		}
	}

	return out;
}

Vector4 Matrix4x4::lookvector() const {
	Vector4 out;

	out[0] = -values[0][2];
	out[1] = -values[1][2];
	out[2] = -values[2][2];
	out[3] = (float)1;

	return out;
}

Vector4 Matrix4x4::position() const {
	Vector4 out;

	out[0] = values[0][3];
	out[1] = values[1][3];
	out[2] = values[2][3];
	out[3] = (float)1;

	return out;
}

Matrix4x4 Matrix4x4::rotation() const {
	return operator-(position());
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& b) const {
	Matrix4x4 out;

	// using b.values because it is more efficient and we have access to private member variables
	out.values[0][0] = values[0][0] * b.values[0][0] + values[0][1] * b.values[1][0] + values[0][2] * b.values[2][0] + values[0][3] * b.values[3][0];
	out.values[0][1] = values[0][0] * b.values[0][1] + values[0][1] * b.values[1][1] + values[0][2] * b.values[2][1] + values[0][3] * b.values[3][1];
	out.values[0][2] = values[0][0] * b.values[0][2] + values[0][1] * b.values[1][2] + values[0][2] * b.values[2][2] + values[0][3] * b.values[3][2];
	out.values[0][3] = values[0][0] * b.values[0][3] + values[0][1] * b.values[1][3] + values[0][2] * b.values[2][3] + values[0][3] * b.values[3][3];
	out.values[1][0] = values[1][0] * b.values[0][0] + values[1][1] * b.values[1][0] + values[1][2] * b.values[2][0] + values[1][3] * b.values[3][0];
	out.values[1][1] = values[1][0] * b.values[0][1] + values[1][1] * b.values[1][1] + values[1][2] * b.values[2][1] + values[1][3] * b.values[3][1];
	out.values[1][2] = values[1][0] * b.values[0][2] + values[1][1] * b.values[1][2] + values[1][2] * b.values[2][2] + values[1][3] * b.values[3][2];
	out.values[1][3] = values[1][0] * b.values[0][3] + values[1][1] * b.values[1][3] + values[1][2] * b.values[2][3] + values[1][3] * b.values[3][3];
	out.values[2][0] = values[2][0] * b.values[0][0] + values[2][1] * b.values[1][0] + values[2][2] * b.values[2][0] + values[2][3] * b.values[3][0];
	out.values[2][1] = values[2][0] * b.values[0][1] + values[2][1] * b.values[1][1] + values[2][2] * b.values[2][1] + values[2][3] * b.values[3][1];
	out.values[2][2] = values[2][0] * b.values[0][2] + values[2][1] * b.values[1][2] + values[2][2] * b.values[2][2] + values[2][3] * b.values[3][2];
	out.values[2][3] = values[2][0] * b.values[0][3] + values[2][1] * b.values[1][3] + values[2][2] * b.values[2][3] + values[2][3] * b.values[3][3];
	out.values[3][0] = values[3][0] * b.values[0][0] + values[3][1] * b.values[1][0] + values[3][2] * b.values[2][0] + values[3][3] * b.values[3][0];
	out.values[3][1] = values[3][0] * b.values[0][1] + values[3][1] * b.values[1][1] + values[3][2] * b.values[2][1] + values[3][3] * b.values[3][1];
	out.values[3][2] = values[3][0] * b.values[0][2] + values[3][1] * b.values[1][2] + values[3][2] * b.values[2][2] + values[3][3] * b.values[3][2];
	out.values[3][3] = values[3][0] * b.values[0][3] + values[3][1] * b.values[1][3] + values[3][2] * b.values[2][3] + values[3][3] * b.values[3][3];

	return out;
}

Vector4 Matrix4x4::operator*(const Vector4& b) const {
	Vector4 out;

	out[0] = values[0][0] * b[0] + values[0][1] * b[1] + values[0][2] * b[2] + values[0][3] * b[3];
	out[1] = values[1][0] * b[0] + values[1][1] * b[1] + values[1][2] * b[2] + values[1][3] * b[3];
	out[2] = values[2][0] * b[0] + values[2][1] * b[1] + values[2][2] * b[2] + values[2][3] * b[3];
	out[3] = values[3][0] * b[0] + values[3][1] * b[1] + values[3][2] * b[2] + values[3][3] * b[3];

	return out;
}

Matrix4x4 Matrix4x4::operator+(const Vector4& b) const {
	Matrix4x4 out;

	out.values[0][0] = values[0][0];
	out.values[0][1] = values[0][1];
	out.values[0][2] = values[0][2];
	out.values[0][3] = values[0][3] + b[0];
	out.values[1][0] = values[1][0];
	out.values[1][1] = values[1][1];
	out.values[1][2] = values[1][2];
	out.values[1][3] = values[1][3] + b[1];
	out.values[2][0] = values[2][0];
	out.values[2][1] = values[2][1];
	out.values[2][2] = values[2][2];
	out.values[2][3] = values[2][3] + b[2];
	out.values[3][0] = 0;
	out.values[3][1] = 0;
	out.values[3][2] = 0;
	out.values[3][3] = 1;

	return out;
}

Matrix4x4 Matrix4x4::operator-(const Vector4& b) const {
	Matrix4x4 out;

	out.values[0][0] = values[0][0];
	out.values[0][1] = values[0][1];
	out.values[0][2] = values[0][2];
	out.values[0][3] = values[0][3] - b[0];
	out.values[1][0] = values[1][0];
	out.values[1][1] = values[1][1];
	out.values[1][2] = values[1][2];
	out.values[1][3] = values[1][3] - b[1];
	out.values[2][0] = values[2][0];
	out.values[2][1] = values[2][1];
	out.values[2][2] = values[2][2];
	out.values[2][3] = values[2][3] - b[2];
	out.values[3][0] = 0;
	out.values[3][1] = 0;
	out.values[3][2] = 0;
	out.values[3][3] = 1;

	return out;
}

bool Matrix4x4::operator==(const Matrix4x4& b) const {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			if (values[row][column] != b.values[row][column]) {
				return false;
			}
		}
	}
	return true;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& b) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			values[row][column] = b.values[row][column];
		}
	}
	return *this;
}

float* Matrix4x4::operator[](int row) {
	return values[row];
}

const float* Matrix4x4::operator[](int row) const {
	return values[row];
}

float* Matrix4x4::getValues() {
	return *values;
}

std::ostream& Math3D::operator <<(std::ostream &outs, const Matrix4x4 &mat) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			outs << mat[row][column];
			if (column < 3 || row < 3) {
				outs << ", ";
			}
		}
	}
}

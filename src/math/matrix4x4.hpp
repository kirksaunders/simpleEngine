#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <iostream>

#include "math/vector4.hpp"

namespace Math3D {
	class Matrix4x4 {
	 public:
		/*
		Returns identity matrix.
		*/
		Matrix4x4();
		/*
		Returns matrix with a position at the given vector with no rotation.
		*/
		Matrix4x4(const Vector4& vector);
		/*
		Returns matrix at position x, y, z with no rotation.
		*/
		Matrix4x4(float x, float y, float z);
		/*
		Returns matrix with its values set as given ones.
		*/
		Matrix4x4(float x, float y, float z, float r00, float r01, float r02, float r10, float r11, float r12, float r20, float r21, float r22);
		/*
		Returns matrix with rotation converted from the x, y, and z euler angles.
		*/
		static Matrix4x4 fromEuler(float x, float y, float z);
		/*
		Returns a projection matrix from the given FOV, aspect ratio, and near and far clipping values.
		*/
		static Matrix4x4 projection(float fov, float aspectRatio, float near, float far);
		/*
		Returns the inverse of the matrix.
		*/
		Matrix4x4 inverse() const;
		/*
		Returns the matrix transposed.
		*/
		Matrix4x4 transpose() const;
		/*
		Returns the forward unit vector from the matrix's rotation.
		*/
		Vector4 lookvector() const;
		/*
		Returns the position of the matrix.
		*/
		Vector4 position() const;
		/*
		Returns the rotation of the matrix.
		*/
		Matrix4x4 rotation() const;
		/*
		Returns the matrix that results from this matrix being multiplied by another.
		*/
		Matrix4x4 operator*(const Matrix4x4&) const;
		/*
		Returns the vector that results from this matrix being multiplied by a vector.
		*/
		Vector4 operator*(const Vector4&) const;
		/*
		Returns the matrix that results from adding a vector to this matrix.
		*/
		Matrix4x4 operator+(const Vector4&) const;
		/*
		Returns the matrix that results from subtracting a vector from this matrix.
		*/
		Matrix4x4 operator-(const Vector4&) const;

		bool operator==(const Matrix4x4&) const;

		bool operator!=(const Matrix4x4&) const;

		Matrix4x4& operator=(const Matrix4x4&);
		/*
		Allows the matrix's values to be indexed like Matrix[row][column]
		*/
		float* operator[](int);
		const float* operator[](int) const;
		/*
		Returns pointer to the matrix's value array
		*/
		float* getValues();

	 private:
		float values[4][4];
	};

	std::ostream& operator <<(std::ostream &outs, const Matrix4x4 &mat);
}

#endif
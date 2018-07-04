#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
	
namespace Math3D {
	class Vector4 {
	 public:
        /*
            Constructs a vector at the origin.
        */
        Vector4();
        /*
            Constructs a vector with the position x, y, z.
        */
        Vector4(float x, float y, float z);
        /*
            Returns the dot product of the vectors.
        */
        float dot(const Vector4&) const;
        /*
            Returns the cross product of the vectors.
        */
        Vector4 cross(const Vector4&) const;
        /*
            Returns the magnitude of the vector.
        */
        float magnitude() const;
        /*
            Returns the normalized form of the vector.
        */
        Vector4 unit() const;
        /*
            Returns the vector multiplied by a scalar.
        */
        Vector4 operator*(float a) const;
        /*
            Returns the vector multiplied by another vector.
        */
        Vector4 operator*(const Vector4&) const;
        /*
            Returns the vector divided by a scalar.
        */
        Vector4 operator/(float a) const;
        /*
            Returns the vector divided by another vector.
        */
        Vector4 operator/(const Vector4&) const;
        /*
            Returns the resultant(sum) of the vectors.
        */
        Vector4 operator+(const Vector4&) const;
        /*
            Returns the difference of the vectors.
        */
        Vector4 operator-(const Vector4&) const;

        bool operator==(const Vector4&) const;
        /*
            Allows the vector's values to be indexed like Vector[row]
        */
        float& operator[](int);
        const float operator[](int) const;
        /*
        Returns pointer to the vector's value array
        */
        float* getValues();

     private:
		float values[4];
	};

	std::ostream& operator <<(std::ostream &outs, const Vector4 &vec);
}

#endif
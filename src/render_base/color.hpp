#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

namespace Render3D {
	class Color {
	 public:
		Color();

		Color(float r, float g, float b);

		Color(float r, float g, float b, float a);

		void setRed(const float r);

		float getRed() const;

		void setGreen(const float g);

		float getGreen() const;

		void setBlue(const float b);

		float getBlue() const;

		void setAlpha(const float a);

		float getAlpha() const;

		Color operator*(float a) const;

		Color operator/(float a) const;

		bool operator==(const Color&) const;

		bool operator!=(const Color&) const;

		float& operator[](int);
		const float operator[](int) const;

		float* getValues();

	 private:
		float values[4];
	};

	std::ostream& operator <<(std::ostream &outs, const Color &col);
}

#endif
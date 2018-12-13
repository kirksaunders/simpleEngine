#include <cmath>

#include "math/constants.hpp"

#include "render_base/color.hpp"

using namespace Render3D;

Color::Color() {
    values[0] = values[1] = values[2] = values[3] = 0.0;
}

Color::Color(float r, float g, float b) {
    values[0] = r;
    values[1] = g;
    values[2] = b;
    values[3] = 1.0;
}

Color::Color(float r, float g, float b, float a) {
    values[0] = r;
    values[1] = g;
    values[2] = b;
    values[3] = a;
}

void Color::setRed(float r) {
    values[0] = r;
}

float Color::getRed() const {
    return values[0];
}

void Color::setGreen(float g) {
    values[1] = g;
}

float Color::getGreen() const {
    return values[1];
}

void Color::setBlue(float b) {
    values[2] = b;
}

float Color::getBlue() const {
    return values[2];
}

void Color::setAlpha(float a) {
    values[3] = a;
}

float Color::getAlpha() const {
    return values[3];
}

Color Color::operator*(float a) const {
    return Color(values[0] * a, values[1] * a, values[2] * a, values[3]);
}

Color Color::operator/(float a) const {
    return Color(values[0] / a, values[1] / a, values[2] / a, values[3]);
}

bool Color::operator==(const Color& b) const {
    for (unsigned int i = 0; i < 4; i++) {
        if (fabs(values[i] - b.values[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool Color::operator!=(const Color& b) const {
    for (unsigned int i = 0; i < 4; i++) {
        if (fabs(values[i] - b.values[i]) > EPSILON) {
            return true;
        }
    }
    return false;
}

float& Color::operator[](int i) {
    return values[i];
}

const float Color::operator[](int i) const {
    return values[i];
}

float* Color::getValues() {
    return values;
}

const float* Color::getValues() const {
    return values;
}

std::ostream& Render3D::operator <<(std::ostream &outs, const Color &col) {
    outs << col[0] << ", " << col[1] << ", " << col[2] << ", " << col[3];

    return outs;
}

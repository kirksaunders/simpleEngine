#include "render_base/camera.hpp"

using namespace Render3D;
using namespace Math3D;

void Camera::setCFrame(const Matrix4x4& cfr) {
    cframe = cfr;
}

Matrix4x4 Camera::getCFrame() const {
    return cframe;
}

void Camera::setFieldOfView(float fov) {
    fieldOfView = fov;
}

float Camera::getFieldOfView() const {
    return fieldOfView;
}

void Camera::setNear(float n) {
    near = n;
}

float Camera::getNear() const {
    return near;
}

void Camera::setFar(float f) {
    far = f;
}

float Camera::getFar() const {
    return far;
}

Matrix4x4 Camera::getPerspective(float aspectRatio) const {
    return Matrix4x4::projection(fieldOfView, aspectRatio, near, far);
}

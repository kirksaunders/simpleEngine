#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/matrix4x4.hpp"

namespace Render3D {
    class Camera {
     public:
        Camera() : fieldOfView(50), near(0.1), far(1000) {}

        void setCFrame(const Math3D::Matrix4x4& cframe);

        Math3D::Matrix4x4 getCFrame() const;

        void setFieldOfView(float fov);

        float getFieldOfView() const;

        void setNear(float n);

        float getNear() const;

        void setFar(float f);

        float getFar() const;

        Math3D::Matrix4x4 getPerspective(float aspectRatio) const;

     private:
        Math3D::Matrix4x4 cframe;
        float fieldOfView;
        float near;
        float far;
    };
}

#endif
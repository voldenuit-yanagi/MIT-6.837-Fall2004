#include "perspective_camera.hpp"

#include <cmath>

PerspectiveCamera::PerspectiveCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float angle_) {
    center = center_;
    direction_.Normalize();
    direction = direction_;
    up_.Normalize();
    up = up_;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f::Cross3(up, horizontal, direction);
    up.Normalize();
    angle = angle_;
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    float size = 2*tan(angle/2);
    Vec3f dir = direction + (size*up)*(point.y()-0.5) + (size*horizontal)*(point.x()-0.5);
    dir.Normalize();
//    cout << center << dir << endl;
    return Ray(center, dir);
}

float PerspectiveCamera::getTMin() const {
    return 0;
}

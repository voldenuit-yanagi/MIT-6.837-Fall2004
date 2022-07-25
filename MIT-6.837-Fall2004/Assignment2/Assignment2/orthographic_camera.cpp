#include "orthographic_camera.hpp"

OrthographicCamera::OrthographicCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float size_) {
    center = center_;
    direction_.Normalize();
    direction = direction_;
    up_.Normalize();
    up = up_;
    size = size_;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f::Cross3(up, horizontal, direction);
    up.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    Vec3f orig = center + (size*up)*(point.y()-0.5) + (size*horizontal)*(point.x()-0.5);
    return Ray(orig, direction);
}

float OrthographicCamera::getTMin() const {
    return -std::numeric_limits<float>::max();
}

#ifndef perspective_camera_hpp
#define perspective_camera_hpp

#include <stdio.h>

#include "camera.hpp"

class PerspectiveCamera: public Camera {
private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float angle;
public:
    PerspectiveCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float angle_);
    ~PerspectiveCamera() {};
    Ray generateRay(Vec2f point);
    float getTMin() const;
};

#endif /* perspective_camera_hpp */

#ifndef OrthographicCamera_hpp
#define OrthographicCamera_hpp

#include <stdio.h>

#include "camera.hpp"

class OrthographicCamera: public Camera {
private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;
public:
    OrthographicCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float size_);
    ~OrthographicCamera() {};
    Ray generateRay(Vec2f point);
    float getTMin() const;
};

#endif /* OrthographicCamera_hpp */

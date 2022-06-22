#ifndef plane_hpp
#define plane_hpp

#include <stdio.h>

#include "object3d.hpp"
#include "vectors.hpp"
#include "material.hpp"

class Plane: public virtual Object3D {
private:
    Vec3f normal;
    float D;
public:
    Plane(Vec3f &normal_, float d, Material *m) {
        normal_.Normalize();
        normal = normal_;
        D = -d; //nP+D=0
        material = m;
    }
    ~Plane();
    bool intersect(const Ray &r, Hit &h, float tmin);
};

#endif /* plane_hpp */

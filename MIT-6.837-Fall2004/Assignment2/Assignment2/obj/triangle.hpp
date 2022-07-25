#ifndef triangle_hpp
#define triangle_hpp

#include <stdio.h>

#include "object3d.hpp"
#include "vectors.hpp"

class Triangle: public virtual Object3D{
private:
    Vec3f p0;
    Vec3f e1;
    Vec3f e2;
    Vec3f normal;
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
        p0 = a;
        e1 = b - a;
        e2 = c - a;
        material = m;
        Vec3f::Cross3(normal, e1, e2);
        normal.Normalize();
    }
    ~Triangle() {}
    bool intersect(const Ray &r, Hit &h, float tmin);
};

#endif /* triangle_hpp */

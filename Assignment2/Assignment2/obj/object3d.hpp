#ifndef Object3D_hpp
#define Object3D_hpp

#include <stdio.h>

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"

class Object3D {
protected:
    Material *material = nullptr;
public:
    Object3D() {}
    ~Object3D() {}
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    Material* getMeterial() {return material;}
};

#endif /* Object3D_hpp */

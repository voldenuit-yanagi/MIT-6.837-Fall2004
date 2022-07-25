#ifndef Object3D_hpp
#define Object3D_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>

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
    virtual void paint(void) = 0;
};

#endif /* Object3D_hpp */

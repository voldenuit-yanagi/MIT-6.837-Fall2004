#ifndef plane_hpp
#define plane_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>

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
    void paint(void);
};

#endif /* plane_hpp */

#ifndef transform_hpp
#define transform_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>

#include "object3d.hpp"
#include "matrix.hpp"

class Transform: public virtual Object3D{
private:
    Matrix mat;
    Object3D *obj = nullptr;
public:
    Transform(Matrix &m, Object3D *o) {
        mat = m;
        obj = o;
    }
    ~Transform() { delete obj; }
    bool intersect(const Ray &r, Hit &h, float tmin);
    void paint(void);
};

#endif /* transform_hpp */

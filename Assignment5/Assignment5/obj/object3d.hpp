#ifndef Object3D_hpp
#define Object3D_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "boundingbox.hpp"
#include "matrix.hpp"

class Grid;

class Object3D {
protected:
    Material *material = nullptr;
    BoundingBox *box = nullptr;
public:
    Object3D() {}
    ~Object3D() {}
    
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint(void) = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m) {};
    
    Material* getMeterial() { return material; }
    BoundingBox* getBoundingBox() { return box; }
};

#endif /* Object3D_hpp */

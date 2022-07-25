#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>

#include "object3d.hpp"
#include "material.hpp"

class Grid;

class Group: public virtual Object3D {
private:
    Object3D **items = nullptr;
    int num;
public:
    Group(int n);
    ~Group() {
        delete material;
        delete box;
        delete [] items;
    }
    bool intersect(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);
    void paint(void);
    void insertIntoGrid(Grid *g, Matrix *m);
};

#endif /* Group_hpp */

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>

#include "object3d.hpp"
#include "material.hpp"

class Group: public virtual Object3D {
private:
    Object3D **items;
    int num;
public:
    Group(int n) {
        num = n;
        items = new Object3D* [n];
    }
    ~Group() {
        delete [] items;
    }
    bool intersect(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);
};

#endif /* Group_hpp */

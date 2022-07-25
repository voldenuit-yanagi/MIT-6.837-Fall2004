#include <iostream>
using namespace std;

#include "group.hpp"
#include "sphere.hpp"

#define MAX std::numeric_limits<float>::max()

Group::Group(int n) {
    num = n;
    items = new Object3D* [n];
}

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool result = false;
    for (int i=0; i<num; i++) {
        if (items[i]->intersect(r, h, tmin)) {
            result = true;
        }
    }
    return result;
}

void Group::addObject(int index, Object3D *obj) {
    items[index] = obj;
    
    BoundingBox *obj_box = obj->getBoundingBox();
    if (obj_box) {
        if (box) {
            box->Extend(obj_box);
        }
        else {
            box = new BoundingBox(obj_box->getMin(), obj_box->getMax());
        }
    }
}

void Group::paint(void) {
    for (int i=0; i<num; i++) {
        items[i]->paint();
    }
}

void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (int i=0; i<num; i++) {
        items[i]->insertIntoGrid(g, m);
    }
}

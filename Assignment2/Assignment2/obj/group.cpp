#include <iostream>
using namespace std;

#include "group.hpp"
#include "sphere.hpp"

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
}

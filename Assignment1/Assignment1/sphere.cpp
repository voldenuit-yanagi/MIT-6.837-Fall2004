#include "sphere.hpp"

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f r0 = center - r.getOrigin();
    float tp = r0.Dot3(r.getDirection());
    float d2 = r0.Dot3(r0) - tp*tp;
    if (d2 > (radius*radius)) {
        return false;
    }
    float tpp = sqrt(radius*radius - d2);
    float length = r.getDirection().Length();
    float t1 = (tp - tpp)/length;
    float t2 = (tp + tpp)/length;
    if (t1 <= tmin) {
        if (t2 > tmin && t2 < h.getT()) {
            h.set(t2, getMeterial(), r);
            return true;
        }
        else {
            return false;
        }
    }
    else if (t1 < h.getT()) {
        h.set(t1, getMeterial(), r);
        return true;
    }
    else {
        return false;
    }
}

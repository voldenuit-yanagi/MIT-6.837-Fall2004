#include "plane.hpp"

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f rd = r.getDirection();
    Vec3f temp;
    Vec3f::Cross3(temp, rd, normal);
    if (temp.Length() == 0) {
        return false;
    }
    else {
        Vec3f r0 = r.getOrigin();
        float t = -(D + r0.Dot3(normal))/rd.Dot3(normal);
        if (t > tmin && t < h.getT()) {
            h.set(t, getMeterial(), normal, r);
            return true;
        }
        else {
            return false;
        }
    }
}

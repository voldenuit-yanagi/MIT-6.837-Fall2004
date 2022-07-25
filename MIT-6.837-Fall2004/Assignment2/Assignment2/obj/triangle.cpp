#include "triangle.hpp"

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f d = r.getDirection();
    if (d.Dot3(normal) == 0) {
        return false;
    }
    Vec3f s = r.getOrigin() - p0;
    Vec3f s1, s2;
    Vec3f::Cross3(s1, d, e2);
    Vec3f::Cross3(s2, s, e1);
    float t = s2.Dot3(e2) / s1.Dot3(e1);
    float b1 = s1.Dot3(s) / s1.Dot3(e1);
    float b2 = s2.Dot3(d) / s1.Dot3(e1);
    float b0 = 1 - b1 - b2;
    if (t > tmin && t < h.getT() && b1 > 0 && b2 > 0 && b0 > 0) {
        h.set(t, getMeterial(), normal, r);
        return true;
    }
    else {
        return false;
    }
}

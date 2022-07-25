#include "transform.hpp"

bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    Vec4f ori(r.getOrigin(), 1.0);
    Vec4f dir(r.getDirection(), 0.0);
    
    Matrix temp(mat);
    temp.Inverse(temp, 0.001);
    temp.Transform(ori);
    temp.Transform(dir);
    
    Vec3f ori3(ori.x(), ori.y(), ori.z());
    Vec3f dir3(dir.x(), dir.y(), dir.z());
    
    if (obj->intersect(Ray(ori3, dir3), h, tmin)) {
        Vec4f normal(h.getNormal(), 0.0);
        temp.Transpose();
        temp.Transform(normal);
        Vec3f normal3(normal.x(), normal.y(), normal.z());
        normal3.Normalize();
        h.set(h.getT(), h.getMaterial(), normal3, r);
        return true;
    }
    return false;
}

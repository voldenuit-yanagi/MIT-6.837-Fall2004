#include "checker_board.hpp"

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f p = hit.getIntersectionPoint();
    mappingMatrix->Transform(p);
    bool which = int(floor(p[0]) + floor(p[1]) + floor(p[2])) & 1;
    if (!which) {
        return material1->Shade(ray, hit, dirToLight, lightColor);
    }
    else {
        return material2->Shade(ray, hit, dirToLight, lightColor);
    }
}

void CheckerBoard::glSetMaterial(void) const {
    material1->glSetMaterial();
}

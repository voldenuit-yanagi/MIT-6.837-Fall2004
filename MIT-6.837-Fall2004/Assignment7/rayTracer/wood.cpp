#include "wood.hpp"
#include "noise.hpp"

float Wood::calculateM(Vec3f point, int Octaves, float Frequency, float Amplitude) {
    float N = 0;
    int c = 1;
    for (int i = 0; i < Octaves; i++) {
        N += PerlinNoise::noise(c*point.x(), c*point.y(), c*point.z()) / c;
        c *= 2;
    }
    float M = sin(Frequency * point.x() + Amplitude * N);
    M = (M+1)/2;
    M = M < 0 ? 0 : M;
    M = M > 1 ? 1 : M;
    return M;
}

Vec3f Wood::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f p = hit.getIntersectionPoint();
    mappingMatrix->Transform(p);
    float M = calculateM(p, octaves, frequency, amplitude);
    return material1->Shade(ray, hit, dirToLight, lightColor) * M + material2->Shade(ray, hit, dirToLight, lightColor) * (1 - M);
}

void Wood::glSetMaterial(void) const {
    
}

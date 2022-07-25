#include "noise.hpp"

#include "perlin_noise.hpp"

float Noise::calculateN(Vec3f point, int Octaves) {
    float N = 0;
    int c = 1;
    for (int i = 0; i < Octaves; i++) {
        N += PerlinNoise::noise(c*point.x(), c*point.y(), c*point.z()) / c;
        c *= 2;
    }
    N += 0.5;
    N = N < 0 ? 0 : N;
    N = N > 1 ? 1 : N;
    return N;
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f p = hit.getIntersectionPoint();
    mappingMatrix->Transform(p);
    float N = calculateN(p, octaves);
    return material1->Shade(ray, hit, dirToLight, lightColor) * N + material2->Shade(ray, hit, dirToLight, lightColor) * (1 - N);
}

void Noise::glSetMaterial(void) const {
    
}


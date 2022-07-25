#ifndef noise_hpp
#define noise_hpp

#include <stdio.h>

#include "material.hpp"
#include "matrix.hpp"
#include "perlin_noise.hpp"

class Noise: public virtual Material {
private:
    Matrix *mappingMatrix;
    Material *material1, *material2;
    int octaves;
public:
    static float calculateN(Vec3f point, int Octaves);
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves_): Material(Vec3f(1, 1, 1)), mappingMatrix(m), material1(mat1), material2(mat2), octaves(octaves_) {}
    ~Noise() { delete mappingMatrix; delete material1; delete material2; }
    
    Vec3f getDiffuseColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float N = calculateN(point, octaves);
        return material1->getDiffuseColor() * N + material2->getDiffuseColor() * (1 - N);
    }
    Vec3f getSpecularColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float N = calculateN(point, octaves);
        return material1->getSpecularColor() * N + material2->getSpecularColor() * (1 - N);
    }
    Vec3f getReflectiveColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float N = calculateN(point, octaves);
        return material1->getReflectiveColor() * N + material2->getReflectiveColor() * (1 - N);
    }
    Vec3f getTransparentColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float N = calculateN(point, octaves);
        return material1->getTransparentColor() * N + material2->getTransparentColor() * (1 - N);
    }
    float getIndexOfRefraction(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float N = calculateN(point, octaves);
        return material1->getIndexOfRefraction() * N + material2->getIndexOfRefraction() * (1 - N);
    }
    
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
};

#endif /* noise_hpp */

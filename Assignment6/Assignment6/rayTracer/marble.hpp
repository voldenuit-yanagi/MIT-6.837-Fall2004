#ifndef marble_hpp
#define marble_hpp

#include <stdio.h>
#include "material.hpp"
#include "perlin_noise.hpp"
#include "matrix.hpp"

class Marble: public virtual Material {
private:
    Matrix *mappingMatrix;
    Material *material1, *material2;
    int octaves;
    float frequency;
    float amplitude;
public:
    static float calculateM(Vec3f point, int Octaves, float Frequency, float Amplitude);
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves_, float frequency_, float amplitude_): Material(Vec3f(1, 1, 1)), mappingMatrix(m), material1(mat1), material2(mat2), octaves(octaves_), frequency(frequency_), amplitude(amplitude_) {}
    ~Marble() { delete mappingMatrix; delete material1; delete material2; }
    
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    
    Vec3f getDiffuseColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float M = calculateM(point, octaves, frequency, amplitude);
        return material1->getDiffuseColor() * M + material2->getDiffuseColor() * (1 - M);
    }
    Vec3f getSpecularColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float M = calculateM(point, octaves, frequency, amplitude);
        return material1->getSpecularColor() * M + material2->getSpecularColor() * (1 - M);
    }
    Vec3f getReflectiveColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float M = calculateM(point, octaves, frequency, amplitude);
        return material1->getReflectiveColor() * M + material2->getReflectiveColor() * (1 - M);
    }
    Vec3f getTransparentColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float M = calculateM(point, octaves, frequency, amplitude);
        return material1->getTransparentColor() * M + material2->getTransparentColor() * (1 - M);
    }
    float getIndexOfRefraction(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        float M = calculateM(point, octaves, frequency, amplitude);
        return material1->getIndexOfRefraction() * M + material2->getIndexOfRefraction() * (1 - M);
    }
};

#endif /* marble_hpp */

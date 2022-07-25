#ifndef phong_material_hpp
#define phong_material_hpp

#include <stdio.h>

#include "material.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "vectors.hpp"

class PhongMaterial: public virtual Material {
private:
    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
public:
    PhongMaterial(const Vec3f &diffuseColor_, const Vec3f &specularColor_, float exponent_,
                  const Vec3f &reflectiveColor_, const Vec3f &transparentColor_,
                  float indexOfRefraction_): Material(diffuseColor_) {
        specularColor = specularColor_;
        exponent = exponent_;
        reflectiveColor = reflectiveColor_;
        transparentColor = transparentColor_;
        indexOfRefraction = indexOfRefraction_;
    }
    ~PhongMaterial() {}
    
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    
    Vec3f getSpecularColor(Vec3f point = Vec3f(0, 0, 0)) const { return specularColor; }
    Vec3f getReflectiveColor(Vec3f point = Vec3f(0, 0, 0)) const { return reflectiveColor; }
    Vec3f getTransparentColor(Vec3f point = Vec3f(0, 0, 0)) const { return transparentColor; }
    float getIndexOfRefraction(Vec3f point = Vec3f(0, 0, 0)) const { return indexOfRefraction; }
};

#endif /* phong_material_hpp */

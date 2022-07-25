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
public:
    PhongMaterial(const Vec3f &diffuseColor_, const Vec3f &specularColor_, float exponent_): Material(diffuseColor_){
        specularColor = specularColor_;
        exponent = exponent_;
    }
    ~PhongMaterial() {}
    
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return specularColor; }
};

#endif /* phong_material_hpp */

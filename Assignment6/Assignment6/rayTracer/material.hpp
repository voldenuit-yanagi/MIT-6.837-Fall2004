#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.hpp"
#include "ray.hpp"
#include "hit.hpp"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

    // CONSTRUCTORS & DESTRUCTOR
    Material(const Vec3f &d_color) { diffuseColor = d_color; }
    virtual ~Material() {}

    // ACCESSORS
    virtual Vec3f getDiffuseColor(Vec3f point = Vec3f(0, 0, 0)) const { return diffuseColor; }
    virtual Vec3f getSpecularColor(Vec3f point = Vec3f(0, 0, 0)) const = 0;
    virtual Vec3f getReflectiveColor(Vec3f point = Vec3f(0, 0, 0)) const = 0;
    virtual Vec3f getTransparentColor(Vec3f point = Vec3f(0, 0, 0)) const = 0;
    virtual float getIndexOfRefraction(Vec3f point = Vec3f(0, 0, 0)) const = 0;
    
    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;
    
    virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

// ====================================================================
// ====================================================================

#endif

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "phong_material.hpp"
#include "extern.hpp"

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight
// artifact for small specular exponents (wide specular lobe)
// ====================================================================

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.hpp"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

void PhongMaterial::glSetMaterial(void) const {

  GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat specular[4] = {
    getSpecularColor().r(),
    getSpecularColor().g(),
    getSpecularColor().b(),
    1.0};
  GLfloat diffuse[4] = {
    getDiffuseColor().r(),
    getDiffuseColor().g(),
    getDiffuseColor().b(),
    1.0};
  
  // NOTE: GL uses the Blinn Torrance version of Phong...
  float glexponent = exponent;
  if (glexponent < 0) glexponent = 0;
  if (glexponent > 128) glexponent = 128;
  
#if !SPECULAR_FIX
    
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  
#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    
  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f normal = hit.getNormal();
    float dot = ray.getDirection().Dot3(normal);
    if (shade_back && dot > 0) {
        normal = normal * (-1.0f);
    }
    
    float nl = max(normal.Dot3(dirToLight), (float)0);
    Vec3f diffuse = nl * getDiffuseColor();
    
    Vec3f halfway = dirToLight - ray.getDirection();
    halfway.Normalize();
    Vec3f specular = pow(normal.Dot3(halfway), exponent) * getSpecularColor();
    specular *= nl;
    
    Vec3f result = (diffuse + specular) * lightColor;
    
    return result;
}

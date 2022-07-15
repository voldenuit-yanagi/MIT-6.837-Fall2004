#define GL_SILENCE_DEPRECATION

#include "perspective_camera.hpp"

#include "matrix.hpp"
#include "extern.hpp"

PerspectiveCamera::PerspectiveCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float angle_): center(center_), direction(direction_), up(up_), angle(angle_) {
    Vec3f::Cross3(horizontal, direction, up);
    Vec3f::Cross3(screenUp, horizontal, direction);
    direction.Normalize();
    up.Normalize();
    horizontal.Normalize();
    screenUp.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
//    float h = 2 * tan(angle/2);
//    float w = h * width/height;
//    Vec3f dir = direction + (h*screenUp)*(point.y()-0.5) + (w*horizontal)*(point.x()-0.5);
//    Vec3f dir = direction*(1.0/tan(angle*0.5)) + screenUp*(2*point.y()-1) + (width/height)*horizontal*(2*point.x()-1);
    Vec3f dir = direction*(1.0/tan(angle*0.5)) + (height*1.0/width)*screenUp*(2*point.y()-1) + horizontal*(2*point.x()-1);
    dir.Normalize();
    return Ray(center, dir);
}

float PerspectiveCamera::getTMin() const {
    return 0;
}

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================
void PerspectiveCamera::glInit(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================
void PerspectiveCamera::glPlaceCamera(void) {
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================
void PerspectiveCamera::dollyCamera(float dist) {
  center += direction*dist;
  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================
void PerspectiveCamera::truckCamera(float dx, float dy) {
  center += horizontal*dx + screenUp*dy;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================
void PerspectiveCamera::rotateCamera(float rx, float ry) {
  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  direction.Normalize();

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
  rotMat.TransformDirection(up);
  up.Normalize();
  rotMat.TransformDirection(screenUp);
  screenUp.Normalize();
  rotMat.TransformDirection(horizontal);
  horizontal.Normalize();
}


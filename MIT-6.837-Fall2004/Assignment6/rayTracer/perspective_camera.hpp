#ifndef perspective_camera_hpp
#define perspective_camera_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "camera.hpp"

class PerspectiveCamera: public Camera {
private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f screenUp;
    Vec3f horizontal;
    float angle;
public:
    PerspectiveCamera(Vec3f &center_, Vec3f &direction_, Vec3f &up_, float angle_);
    ~PerspectiveCamera() {};
    
    Ray generateRay(Vec2f point);
    float getTMin() const;
    
    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);
};

#endif /* perspective_camera_hpp */

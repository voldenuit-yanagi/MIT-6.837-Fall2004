#ifndef surface_of_revolution_hpp
#define surface_of_revolution_hpp

#include <stdio.h>

#include "surface.hpp"
#include "curve.hpp"

class SurfaceOfRevolution: public Surface {
private:
    Curve *curve;
public:
    SurfaceOfRevolution(Curve *c): curve(c) {}
    ~SurfaceOfRevolution() {}
    
    void set(int i, const Vec3f &v) {
        curve->set(i, v);
    }
    
    // FOR VISUALIZATION
    void Paint(ArgParser *args) {
        curve->Paint(args);
    }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file) {
        curve->OutputBezier(file);
    }
    void OutputBSpline(FILE *file) {
        curve->OutputBSpline(file);
    }

    // FOR CONTROL POINT PICKING
    int getNumVertices() {
        return curve->getNumVertices();
    }
    Vec3f getVertex(int i) {
        return curve->getVertex(i);
    }

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y) {
        curve->moveControlPoint(selectedPoint, x, y);
    }
    void addControlPoint(int selectedPoint, float x, float y) {
        curve->addControlPoint(selectedPoint, x, y);
    }
    void deleteControlPoint(int selectedPoint) {
        curve->deleteControlPoint(selectedPoint);
    }

    // FOR GENERATING TRIANGLES
    TriangleMesh* OutputTriangles(ArgParser *args) {
        return curve->OutputTriangles(args);
    }
};

#endif /* surface_of_revolution_hpp */

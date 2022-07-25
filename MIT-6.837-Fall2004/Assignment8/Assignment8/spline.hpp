#ifndef spline_hpp
#define spline_hpp

#include <stdio.h>

#include "arg_parser.hpp"
#include "vectors.hpp"
#include "triangle_mesh.hpp"
#include "matrix.hpp"

class Spline {
protected:
    static Matrix Bezier_Matrix;
    static Matrix BSpline_Matrix;
public:
    Spline() {}
    
    virtual void set(int i, const Vec3f &v) {}
    
    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) {}
    virtual void OutputBSpline(FILE *file) {}

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() = 0;
    virtual Vec3f getVertex(int i) = 0;

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) {}
    virtual void addControlPoint(int selectedPoint, float x, float y) {}
    virtual void deleteControlPoint(int selectedPoint) {}

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args) = 0;
};

#endif /* spline_hpp */

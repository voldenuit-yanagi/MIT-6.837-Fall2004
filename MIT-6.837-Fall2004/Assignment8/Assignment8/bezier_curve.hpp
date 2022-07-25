#ifndef bezier_curve_hpp
#define bezier_curve_hpp

#include <stdio.h>

#include "curve.hpp"

class BezierCurve: public Curve {
private:
    //
public:
    BezierCurve(int _num_vertices): Curve(_num_vertices) {}
    ~BezierCurve() {}
    
    void drawCurve(int tessellation);

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file);
    void OutputBSpline(FILE *file);

    // FOR GENERATING TRIANGLES
    TriangleMesh* OutputTriangles(ArgParser *args);
};

#endif /* bezier_curve_hpp */

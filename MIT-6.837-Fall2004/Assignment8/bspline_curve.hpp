#ifndef bspline_curve_hpp
#define bspline_curve_hpp

#include <stdio.h>

#include "curve.hpp"

class BSplineCurve: public Curve {
private:
    //
public:
    BSplineCurve(int _num_vertices): Curve(_num_vertices) {}
    ~BSplineCurve() {}
    
    void drawCurve(int tessellation);

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file);
    void OutputBSpline(FILE *file);

    // FOR GENERATING TRIANGLES
    TriangleMesh* OutputTriangles(ArgParser *args);
};

#endif /* bspline_curve_hpp */

#ifndef bezier_patch_hpp
#define bezier_patch_hpp

#include <stdio.h>
#include <vector>

#include "surface.hpp"

class BezierPatch: public Surface {
private:
    int num_vertices;
    vector<Vec3f> vertices;
public:
    BezierPatch() {
        num_vertices = 16;
        vertices.resize(num_vertices);
    }
    ~BezierPatch() {}
    
    void set(int i, const Vec3f &v) {
        vertices[i] = v;
    }
    
    // FOR VISUALIZATION
    void Paint(ArgParser *args);

    // FOR CONTROL POINT PICKING
    int getNumVertices() {
        return num_vertices;
    }
    Vec3f getVertex(int i) {
        return vertices[i];
    }

    // FOR GENERATING TRIANGLES
    TriangleMesh* OutputTriangles(ArgParser *args);
};

#endif /* bezier_patch_hpp */

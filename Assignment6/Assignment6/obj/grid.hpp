#ifndef grid_hpp
#define grid_hpp

#include <stdio.h>
#include "unordered_map"

#include "object3d.hpp"
#include "phong_material.hpp"
#include "object3dvector.hpp"
#include "raytracing_stats.hpp"
#include "transform.hpp"

struct MarchingInfo {
    float tMin;
    int i=-1, j=-1, k=-1;
    float t_next_x, t_next_y, t_next_z;
    float t_next_min;
    float d_tx, d_ty, d_tz;
    int sign_x, sign_y, sign_z;
    Vec3f face_normal;
    int face_index;
    
    void nextCell() {
        RayTracingStats::IncrementNumGridCellsTraversed();
    
        if (t_next_min == t_next_x) {
            i += sign_x;
            tMin = t_next_x;
            t_next_x += d_tx;
            if (sign_x > 0) {
                face_normal = Vec3f(-1, 0, 0);
                face_index = 1;
            }
            else {
                face_normal = Vec3f(1, 0, 0);
                face_index = 0;
            }
        }
        else if (t_next_min == t_next_y) {
            j += sign_y;
            tMin = t_next_y;
            t_next_y += d_ty;
            if (sign_y > 0) {
                face_normal = Vec3f(0, -1, 0);
                face_index = 3;
            }
            else {
                face_normal = Vec3f(0, 1, 0);
                face_index = 2;
            }
        }
        else {
            k += sign_z;
            tMin = t_next_z;
            t_next_z += d_tz;
            if (sign_z > 0) {
                face_normal = Vec3f(0, 0, -1);
                face_index = 5;
            }
            else {
                face_normal = Vec3f(0, 0, 1);
                face_index = 4;
            }
        }
        t_next_min = min({t_next_x, t_next_y, t_next_z});
    }
};

class Grid: public virtual Object3D {
private:
    int nx;
    int ny;
    int nz;
    Object3DVector *record;
    Object3DVector plane_record;
    unordered_map<Object3D*, Transform*> accumulated_transformations;
    Vec3f unit_cell;
    Vec3f point_offset[8] = {Vec3f(0.5, 0.5, -0.5), Vec3f(0.5, 0.5, 0.5),
                             Vec3f(0.5, -0.5, 0.5), Vec3f(0.5, -0.5, -0.5),
                             Vec3f(-0.5, 0.5, -0.5), Vec3f(-0.5, 0.5, 0.5),
                             Vec3f(-0.5, -0.5, 0.5), Vec3f(-0.5, -0.5, -0.5)};
    Material *numIndicativeMaterials[11];
    Material *seqIndicativeMaterials[11];
    bool inRecord(int i, int j, int k);
public:
    Grid(BoundingBox *bb, int nx_, int ny_, int nz_) {
        box = bb;
        nx = nx_;
        ny = ny_;
        nz = nz_;
        record = new Object3DVector [nx*ny*nz]();
        unit_cell = box->getMax() - box->getMin();
        unit_cell.Divide(nx, ny, nz);
        for (int i=0; i<8; i++) {
            point_offset[i] = point_offset[i] * unit_cell;
        }
        
        material = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0,
                                     Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0);
        numIndicativeMaterials[0] = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0,
                                                   Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0);
        seqIndicativeMaterials[0] = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0,
                                                   Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0);
        for (int i=1; i<16; i++) {
            numIndicativeMaterials[i] = new PhongMaterial(Vec3f(rand()%256, rand()%256, rand()%256)*(1./256.), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0);
            if (i<11) {
                seqIndicativeMaterials[i] = new PhongMaterial(Vec3f(259.22-13.22*i, 256.67-10.67*i, 203.11-4.11*i)*(1./256.), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0);
            }
        }
    }
    ~Grid() { delete material; delete box; }
    
    bool intersect(const Ray &r, Hit &h, float tmin);
    void paint(void);
    
    int get_nx() { return nx; }
    int get_ny() { return ny; }
    int get_nz() { return nz; }
    Vec3f getVoxelCenter(int i, int j, int k);
    Vec3f getUnitCell() { return unit_cell; }
    
    void addRecord(int i, int j, int k, Object3D *o);
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
    void addPlaneRecord(Object3D *o);
    void addTransformation(Object3D* obj, Matrix* mat);
};

#endif /* grid_hpp */

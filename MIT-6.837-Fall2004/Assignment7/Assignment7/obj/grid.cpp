#include "grid.hpp"
#include "rayTree.hpp"
#include "extern.hpp"

#include "unordered_map"
#include <iostream>
#include <iomanip>
using std::setprecision;

Vec3f normals[] = {Vec3f(1, 0, 0), Vec3f(-1, 0, 0),
                   Vec3f(0, 1, 0), Vec3f(0, -1, 0),
                   Vec3f(0, 0, 1), Vec3f(0, 0, -1)};

int faces[6][4] = {{0, 1, 2, 3}, {5, 4, 7, 6},
                   {0, 4, 5, 1}, {7, 3, 2, 6},
                   {1, 5, 6, 2}, {3, 7, 4, 0}};

bool Grid::inRecord(int i, int j, int k) {
    return i>=0 && i<nx && j>=0 && j<ny && k>=0 && k<nz;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin) {
    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);
    
    unordered_map<Object3D*, Hit*> mark;
    
    bool res = false;
    int cnt = 0;
    while (inRecord(mi.i, mi.j, mi.k)) {
        int numObj = record[nx*ny*mi.k + nx*mi.j + mi.i].getNumObjects();
        if (visualize_grid) {
            if (!res && numObj > 0) {
                h.set(mi.tMin, numIndicativeMaterials[min(numObj, 15)], mi.face_normal, r);
                res = true;
            }
            Vec3f center = getVoxelCenter(mi.i, mi.j, mi.k);
            for (int f = 0; f < 6; f++) {
                int *p = faces[f];
                RayTree::AddHitCellFace(center + point_offset[p[0]],
                                        center + point_offset[p[1]],
                                        center + point_offset[p[2]],
                                        center + point_offset[p[3]],
                                        normals[f], seqIndicativeMaterials[min(cnt, 10)]);
                if (f == mi.face_index) {
                    RayTree::AddEnteredFace(center + point_offset[p[0]],
                                            center + point_offset[p[1]],
                                            center + point_offset[p[2]],
                                            center + point_offset[p[3]],
                                            mi.face_normal, seqIndicativeMaterials[min(cnt, 10)]);
                }
            }
        }
        else {
            if (!res && numObj > 0) {
                for (int i=0; i<numObj; i++) {
                    Object3D* obj = record[nx*ny*mi.k + nx*mi.j + mi.i].getObject(i);
                    if (mark.find(obj) == mark.end()) {
                        Hit *tempHit = new Hit();
                        obj->intersect(r, *tempHit, tmin);
                        mark[obj] = tempHit;
                    }
                    float t = mark[obj]->getT();
                    if (t-mi.tMin >= -1e-5 && (t-mi.t_next_min) <= 1e-5 && t < h.getT()) {
                        h.set(t, mark[obj]->getMaterial(), mark[obj]->getNormal(), r);
                        res = true;
                    }
                }
                if (res) {
                    break;
                }
            }
        }
        mi.nextCell();
        cnt++;
    }
    if (!visualize_grid) {
        for (int i=0; i<plane_record.getNumObjects(); i++) {
            if (plane_record.getObject(i)->intersect(r, h, tmin)) {
                res = true;
            }
        }
    }
    return res;
}

int findMinimumIndex(float *arr, int length) {
    int index = 0;
    for (int i=1; i<length; i++) {
        if (arr[i] < arr[index]) {
            index = i;
        }
    }
    return index;
}

int findMaximumIndex(float *arr, int length) {
    int index = 0;
    for (int i=1; i<length; i++) {
        if (arr[i] > arr[index]) {
            index = i;
        }
    }
    return index;
}

void Grid::paint(void) {
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny; j++) {
            for (int k=0; k<nz; k++) {
                int numObj = record[nx*ny*k + nx*j + i].getNumObjects();
                if (numObj > 0) {
                    numIndicativeMaterials[min(numObj, 15)]->glSetMaterial();
                    glBegin(GL_QUADS);
                    Vec3f center = getVoxelCenter(i, j, k);
                    for (int f = 0; f < 6; f++) {
                        glNormal3f(normals[f].x(), normals[f].y(), normals[f].z());
                        for (int p: faces[f]) {
                            Vec3f point = center + point_offset[p];
                            glVertex3f(point.x(), point.y(), point.z());
                        }
                    }
                    glEnd();
                }
            }
        }
    }
}

Vec3f Grid::getVoxelCenter(int i, int j, int k) {
    return box->getMin() + Vec3f(i+0.5, j+0.5, k+0.5) * unit_cell;
}

void Grid::addRecord(int i, int j, int k, Object3D *o) {
    record[nx*ny*k + nx*j + i].addObject(o);
}

void Grid::addPlaneRecord(Object3D *o) {
    plane_record.addObject(o);
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const {
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();
    Vec3f boxMin = box->getMin();
    Vec3f t1 = (boxMin - origin);
    Vec3f t2 = (box->getMax() - origin);
    float txmin = min(t1.x()/direction.x(), t2.x()/direction.x());
    float txmax = max(t1.x()/direction.x(), t2.x()/direction.x());
    float tymin = min(t1.y()/direction.y(), t2.y()/direction.y());
    float tymax = max(t1.y()/direction.y(), t2.y()/direction.y());
    float tzmin = min(t1.z()/direction.z(), t2.z()/direction.z());
    float tzmax = max(t1.z()/direction.z(), t2.z()/direction.z());
    
    float minArr[] = {txmin, tymin, tzmin};
    int maxIndex_minArr = findMaximumIndex(minArr, 3);
    float maxArr[] = {txmax, tymax, tzmax};
    int minIndex_maxArr = findMinimumIndex(maxArr, 3);
    
    float tnear = minArr[maxIndex_minArr];
    float tfar = maxArr[minIndex_maxArr];
    
    if (tnear < tfar and tfar > tmin) {
        mi.sign_x = direction.x() > 0 ? 1: -1;
        mi.sign_y = direction.y() > 0 ? 1: -1;
        mi.sign_z = direction.z() > 0 ? 1: -1;
        
        mi.d_tx = unit_cell.x() * mi.sign_x / direction.x();
        mi.d_ty = unit_cell.y() * mi.sign_y / direction.y();
        mi.d_tz = unit_cell.z() * mi.sign_z / direction.z();
        
        int index;
        if (tnear > tmin) {
            mi.tMin = tnear;
            index = maxIndex_minArr;
        }
        else {
            mi.tMin = tmin;
            index = minIndex_maxArr;
        }
        
        Vec3f relativePosition = r.pointAtParameter(mi.tMin) - boxMin;
        mi.i = relativePosition.x() / unit_cell.x();
        mi.i = mi.i==nx ? nx-1 : mi.i;
        mi.j = relativePosition.y() / unit_cell.y();
        mi.j = mi.j==ny ? ny-1 : mi.j;
        mi.k = relativePosition.z() / unit_cell.z();
        mi.k = mi.k==nz ? nz-1 : mi.k;
        
        mi.t_next_x = mi.sign_x>0 ? ((mi.i+1)*unit_cell.x()+boxMin.x()-origin.x())/direction.x() : (mi.i*unit_cell.x()+boxMin.x()-origin.x())/direction.x();
        mi.t_next_y = mi.sign_y>0 ? ((mi.j+1)*unit_cell.y()+boxMin.y()-origin.y())/direction.y() : (mi.j*unit_cell.y()+boxMin.y()-origin.y())/direction.y();
        mi.t_next_z = mi.sign_z>0 ? ((mi.k+1)*unit_cell.z()+boxMin.z()-origin.z())/direction.z() : (mi.k*unit_cell.z()+boxMin.z()-origin.z())/direction.z();
        
        if (index == 0) {
            if (mi.sign_x > 0) {
                mi.face_index = 1;
                mi.face_normal = normals[1];
            }
            else {
                mi.face_index = 0;
                mi.face_normal = normals[0];
            }
        }
        else if (index == 1) {
            if (mi.sign_y > 0) {
                mi.face_index = 3;
                mi.face_normal = normals[3];
            }
            else {
                mi.face_index = 2;
                mi.face_normal = normals[2];
            }
        }
        else {
            if (mi.sign_z > 0) {
                mi.face_index = 5;
                mi.face_normal = normals[5];
            }
            else {
                mi.face_index = 4;
                mi.face_normal = normals[4];
            }
        }
        mi.t_next_min = min({mi.t_next_x, mi.t_next_y, mi.t_next_z});
    }
}
